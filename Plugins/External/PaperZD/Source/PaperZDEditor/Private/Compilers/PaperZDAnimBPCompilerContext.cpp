// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Compilers/PaperZDAnimBPCompilerContext.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "Compilers/Access/PaperZDAnimBPGeneratedClassAccess.h"
#include "Editors/Util/PaperZDEditorSettings.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "PaperZDAnimBP.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimBPGeneratedClass.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Graphs/PaperZDNotifyGraph.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Sink.h"
#include "EdGraphUtilities.h"
#include "Kismet2/KismetReinstanceUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_Knot.h"
#include "PaperZDCustomVersion.h"

#define LOCTEXT_NAMESPACE "PaperZDAnimBlueprintCompiler"

//static definitions
TMap<zid_t, TUniquePtr<IPaperZDCompilerHandleFactory>> FPaperZDAnimBPCompilerContext::RegisteredHandleFactories = TMap<zid_t, TUniquePtr<IPaperZDCompilerHandleFactory>>();
//end static definitions

FPaperZDAnimBPCompilerContext::FPaperZDAnimBPCompilerContext(UBlueprint* Blueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions) 
	: FKismetCompilerContext(Blueprint, InMessageLog, InCompilerOptions)
	, AnimBP(CastChecked<UPaperZDAnimBP>(Blueprint))
{
	//Initialize all the compiler handles
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	for(auto FactoryIt = RegisteredHandleFactories.CreateConstIterator(); FactoryIt; ++FactoryIt)
	{
		zid_t HandleId = 0;
		IPaperZDCompilerHandleFactory* FactoryPtr = (*FactoryIt).Value.Get();
		IPaperZDAnimBPCompilerHandle* Handle = FactoryPtr->CreateNew(CompilerAccess, HandleId);
		
		CompileHandles.Add(HandleId, TUniquePtr<IPaperZDAnimBPCompilerHandle>(Handle));
	}

	//Add the AnimBP graph schema as a known schema
	KnownGraphSchemas.AddUnique(UPaperZDAnimGraphSchema::StaticClass());
}

TUniquePtr<FPaperZDAnimBPCompilerAccess> FPaperZDAnimBPCompilerContext::RequestAccess(FKismetCompilerContext& CompilerContext)
{
	FPaperZDAnimBPCompilerContext* ZDContext = static_cast<FPaperZDAnimBPCompilerContext*>(&CompilerContext);
	if (ZDContext)
	{
		FPaperZDAnimBPCompilerAccess* Access = new FPaperZDAnimBPCompilerAccess(ZDContext);
		return TUniquePtr<FPaperZDAnimBPCompilerAccess>(Access);
	}

	return TUniquePtr<FPaperZDAnimBPCompilerAccess>();
}

void FPaperZDAnimBPCompilerContext::ForAllSubGraphs(UEdGraph* InGraph, TFunctionRef<void(UEdGraph*)> InPerGraphFunction)
{
	TArray<UEdGraph*> AllGraphs;
	AllGraphs.Add(InGraph);
	InGraph->GetAllChildrenGraphs(AllGraphs);

	for (UEdGraph* CurrGraph : AllGraphs)
	{
		InPerGraphFunction(CurrGraph);
	}
};

void FPaperZDAnimBPCompilerContext::PruneIsolatedAnimationNodes(const TArray<UPaperZDAnimGraphNode_Base*>& RootSet, TArray<UPaperZDAnimGraphNode_Base*>& GraphNodes)
{
	//Prune any animation node that has been orphaned
	struct FNodeVisitorDownAnimWires
	{
		TSet<UEdGraphNode*> VisitedNodes;
		const UPaperZDAnimGraphSchema* Schema;

		FNodeVisitorDownAnimWires()
		{
			Schema = GetDefault<UPaperZDAnimGraphSchema>();
		}

		void TraverseNodes(UEdGraphNode* Node)
		{
			VisitedNodes.Add(Node);

			// Follow every exec output pin
			for (int32 i = 0; i < Node->Pins.Num(); ++i)
			{
				UEdGraphPin* MyPin = Node->Pins[i];

				if ((MyPin->Direction == EGPD_Input) && (Schema->IsAnimationDataPin(MyPin->PinType)))
				{
					for (int32 j = 0; j < MyPin->LinkedTo.Num(); ++j)
					{
						UEdGraphPin* OtherPin = MyPin->LinkedTo[j];
						UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
						if (!VisitedNodes.Contains(OtherNode))
						{
							TraverseNodes(OtherNode);
						}
					}
				}
			}
		}
	};

	// Prune the nodes that aren't reachable via an animation pose link
	FNodeVisitorDownAnimWires Visitor;

	for (auto RootIt = RootSet.CreateConstIterator(); RootIt; ++RootIt)
	{
		UPaperZDAnimGraphNode_Base* RootNode = *RootIt;
		Visitor.TraverseNodes(RootNode);
	}

	for (int32 NodeIndex = 0; NodeIndex < GraphNodes.Num(); ++NodeIndex)
	{
		UPaperZDAnimGraphNode_Base* Node = GraphNodes[NodeIndex];

		// Remove every node that hasn't been visited, as they are orphaned and won't be run in runtime
		if (!Visitor.VisitedNodes.Contains(Node) && !IsNodePure(Node))
		{
			Node->BreakAllNodeLinks();
			GraphNodes.RemoveAtSwap(NodeIndex);
			--NodeIndex;
		}
	}
}

UEdGraphSchema_K2* FPaperZDAnimBPCompilerContext::CreateSchema()
{
	return NewObject<UPaperZDAnimGraphSchema>();
}

void FPaperZDAnimBPCompilerContext::CreateFunctionList()
{
	//Before continuing, we need to make sure all the AnimNotifies have generated their function stubs and are visible on the editor
	//for this we add them to the AnimBP itself and configure them as generated graphs for the user to fill with logic
	UpdateAnimNotifyFunctions();
	FKismetCompilerContext::CreateFunctionList();
}

void FPaperZDAnimBPCompilerContext::UpdateAnimNotifyFunctions()
{
	//If for some reason this AnimBP doesn't have an animation source connected to it, bail-out.
	if (AnimBP->GetSupportedAnimationSource() == nullptr)
	{
		return;
	}

	//We will map any notify function to its corresponding name on the class, so they can be found easily when running the notify events
	for (int32 i = 0; i < AnimBP->FunctionGraphs.Num(); i++)
	{
		UPaperZDNotifyGraph* AnimNotifyGraph = Cast<UPaperZDNotifyGraph>(AnimBP->FunctionGraphs[i]);
		if (AnimNotifyGraph)
		{
			if (AnimBP->GetSupportedAnimationSource()->GetRegisteredNotifyNames().Find(AnimNotifyGraph->NotifyName) != INDEX_NONE)
			{
				//Notify still exists, pass the actual function name to the generated class
 				TArray<UK2Node_FunctionEntry*> EntryPoints;
				AnimNotifyGraph->GetNodesOfClass(EntryPoints);
 				check(EntryPoints.Num() == 1);
				NewAnimBlueprintClass->AnimNotifyFunctionMapping.Add(AnimNotifyGraph->NotifyName, EntryPoints[0]->CustomGeneratedFunctionName);
			}
		}
	}
}

void FPaperZDAnimBPCompilerContext::GetLinkedAnimNodes(UPaperZDAnimGraphNode_Base* InGraphNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const
{
	for (UEdGraphPin* Pin : InGraphNode->Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input &&
			Pin->PinType.PinCategory == TEXT("struct"))
		{
			if (UScriptStruct* Struct = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get()))
			{
				if (Struct->IsChildOf(FPaperZDAnimDataLink::StaticStruct()))
				{
					GetLinkedAnimNodes_TraversePin(Pin, LinkedAnimNodes);
				}
			}
		}
	}
}

void FPaperZDAnimBPCompilerContext::GetLinkedAnimNodes_TraversePin(UEdGraphPin* InPin, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const
{
	if (!InPin)
	{
		return;
	}

	for (UEdGraphPin* LinkedPin : InPin->LinkedTo)
	{
		if (!LinkedPin)
		{
			continue;
		}

		UEdGraphNode* OwningNode = LinkedPin->GetOwningNode();

		if (UK2Node_Knot* InnerKnot = Cast<UK2Node_Knot>(OwningNode))
		{
			GetLinkedAnimNodes_TraversePin(InnerKnot->GetInputPin(), LinkedAnimNodes);
		}
		else if (UPaperZDAnimGraphNode_Base* AnimNode = Cast<UPaperZDAnimGraphNode_Base>(OwningNode))
		{
			GetLinkedAnimNodes_ProcessAnimNode(AnimNode, LinkedAnimNodes);
		}
	}
}

void FPaperZDAnimBPCompilerContext::GetLinkedAnimNodes_ProcessAnimNode(UPaperZDAnimGraphNode_Base* AnimNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const
{
	if (!AllocatedAnimNodes.Contains(AnimNode))
	{
		UPaperZDAnimGraphNode_Base* TrueSourceNode = MessageLog.FindSourceObjectTypeChecked<UPaperZDAnimGraphNode_Base>(AnimNode);
		if (UPaperZDAnimGraphNode_Base* const* AllocatedNode = SourceNodeToProcessedNodeMap.Find(TrueSourceNode))
		{
			LinkedAnimNodes.Add(*AllocatedNode);
		}
		else
		{
			FString ErrorString = FText::Format(LOCTEXT("MissingLinkFmt", "Missing allocated node for {0} while searching for node links - likely due to the node having outstanding errors."), FText::FromString(AnimNode->GetName())).ToString();
			MessageLog.Error(*ErrorString);
		}
	}
	else
	{
		LinkedAnimNodes.Add(AnimNode);
	}
}

void FPaperZDAnimBPCompilerContext::MergeUbergraphPagesIn(UEdGraph* Ubergraph)
{
	FKismetCompilerContext::MergeUbergraphPagesIn(Ubergraph);

	//We don't have support for inheriting classes, but if we had, this is the point where we would avoid merging pages.
	{
		// Move all animation graph nodes and associated pure logic chains into the consolidated event graph
		auto MoveGraph = [this](UEdGraph* InGraph)
		{
			if (InGraph->Schema->IsChildOf(UPaperZDAnimGraphSchema::StaticClass()))
			{
				// Merge all the animation nodes, contents, etc... into the ubergraph
				UEdGraph* ClonedGraph = FEdGraphUtilities::CloneGraph(InGraph, NULL, &MessageLog, true);

				// Prune the graph up-front
				const bool bIncludePotentialRootNodes = false;
				PruneIsolatedNodes(ClonedGraph, bIncludePotentialRootNodes);

				const bool bIsLoading = Blueprint->bIsRegeneratingOnLoad || IsAsyncLoading();
				const bool bIsCompiling = Blueprint->bBeingCompiled;
				ClonedGraph->MoveNodesToAnotherGraph(ConsolidatedEventGraph, bIsLoading, bIsCompiling);

				// Move subgraphs too
				ConsolidatedEventGraph->SubGraphs.Append(ClonedGraph->SubGraphs);
			}
		};

		//Because we want to avoid any potential issues or interactions, we manually add the AnimGraph into the consolidated graph
		//The graph by this point has been removed as an "UbergraphPage" for this very reason (but will be added after compilation, for visual purposes)
		//Manually add the AnimationGraph onto the consolidated Ubergraph and process it
		//
		//Note: The Animation Graph could be not compatible with the current compiler due to it being loaded from an older version of the plugin, in this case we
		//avoid compiling the graph and wait for the version patch-up to fix the blueprint.
		if (AnimBP->IsVersionCompatibleForCompilation())
		{
			MoveGraph(AnimBP->AnimationGraph);

			//Expand split pins on the consolidated graph before processing nodes
			ForAllSubGraphs(ConsolidatedEventGraph, [this](UEdGraph* InGraph)
			{
				ExpandSplitPins(InGraph);
			});

			//Finally process the animation nodes
			ProcessAllAnimationNodes();
		}
	}
}

void FPaperZDAnimBPCompilerContext::ProcessOneFunctionGraph(UEdGraph* SourceGraph, bool bInternalFunction /* = false */)
{
	if (!KnownGraphSchemas.FindByPredicate([SourceGraph](const TSubclassOf<UEdGraphSchema>& InSchemaClass)
		{
			return SourceGraph->Schema->IsChildOf(InSchemaClass.Get());
		}))
	{
		// Not known as a schema that this compiler looks at, pass to the default
		FKismetCompilerContext::ProcessOneFunctionGraph(SourceGraph, bInternalFunction);
	}
}

void FPaperZDAnimBPCompilerContext::SpawnNewClass(const FString& NewClassName)
{
	NewAnimBlueprintClass = FindObject<UPaperZDAnimBPGeneratedClass>(Blueprint->GetOutermost(), *NewClassName);

	if (NewAnimBlueprintClass == NULL)
	{
		NewAnimBlueprintClass = NewObject<UPaperZDAnimBPGeneratedClass>(Blueprint->GetOutermost(), FName(*NewClassName), RF_Public | RF_Transactional);
	}
	else
	{
		// Already existed, but wasn't linked in the Blueprint yet due to load ordering issues
		FBlueprintCompileReinstancer::Create(NewAnimBlueprintClass);
	}

	//Setup the class
	NewClass = NewAnimBlueprintClass;
	NewAnimBlueprintClass->SupportedAnimationSource = AnimBP->SupportedAnimationSource;

	//Call delegates
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	OnStartCompilingClass.Broadcast(NewAnimBlueprintClass, CompilerAccess, ClassAccess);
}

void FPaperZDAnimBPCompilerContext::OnNewClassSet(UBlueprintGeneratedClass* ClassToUse)
{
	NewAnimBlueprintClass = CastChecked<UPaperZDAnimBPGeneratedClass>(ClassToUse);
}

void FPaperZDAnimBPCompilerContext::OnPostCDOCompiled(const UObject::FPostCDOCompiledContext& Context)
{
	//We don't support blueprint inheritance for PaperZD, but adding extra measures if we ever need it is pretty easy
	for (UPaperZDAnimBPGeneratedClass* ClassWithInputHandlers = NewAnimBlueprintClass; ClassWithInputHandlers != nullptr; ClassWithInputHandlers = Cast<UPaperZDAnimBPGeneratedClass>(ClassWithInputHandlers->GetSuperClass()))
	{
		FPaperZDExposedValueHandler::InitClass(ClassWithInputHandlers->EvaluateGraphExposedInputs, NewAnimBlueprintClass->ClassDefaultObject);
		ClassWithInputHandlers->CacheRequiredNodes(NewAnimBlueprintClass->ClassDefaultObject);
	}
}

void FPaperZDAnimBPCompilerContext::ProcessAllAnimationNodes()
{
	//Obtain all the graph nodes
	TArray<UPaperZDAnimGraphNode_Base*> RootAnimNodeList;
	ConsolidatedEventGraph->GetNodesOfClass<UPaperZDAnimGraphNode_Base>(RootAnimNodeList);
	
	// We recursively build the node lists for pre- and post-processing phases to make sure
	// we catch any handler-relevant nodes in sub-graphs
	TArray<UPaperZDAnimGraphNode_Base*> AllSubGraphsAnimNodeList;
	ForAllSubGraphs(ConsolidatedEventGraph, [&AllSubGraphsAnimNodeList](UEdGraph* InGraph)
	{
		InGraph->GetNodesOfClass<UPaperZDAnimGraphNode_Base>(AllSubGraphsAnimNodeList);
	});

	// Find the root nodes
	TArray<UPaperZDAnimGraphNode_Base*> RootSet;
	AllocateNodeIndexCounter = 0;

	for (UPaperZDAnimGraphNode_Base* SourceNode : RootAnimNodeList)
	{
		//UPaperZDAnimGraphNode_Base* TrueNode = MessageLog.FindSourceObjectTypeChecked<UPaperZDAnimGraphNode_Base>(SourceNode);
		//TrueNode->BlueprintUsage = EBlueprintUsage::NoProperties; //@NOTE: To fill if we have fast pathing

		if (SourceNode->IsNodeRootSet())
		{
			RootSet.Add(SourceNode);
		}
	}

	if (RootAnimNodeList.Num() > 0)
	{
		// Prune any anim nodes (they will be skipped by PruneIsolatedNodes above)
		PruneIsolatedAnimationNodes(RootSet, RootAnimNodeList);

		// Validate the graph
		ValidateGraphIsWellFormed(ConsolidatedEventGraph);
		
		//Notify any delegate that might need to do something before the data has been processed
		FPaperZDAnimBPCompilerAccess CompilerAccess(this);
		FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
		OnPreProcessAnimationNodes.Broadcast(AllSubGraphsAnimNodeList, CompilerAccess, ClassAccess);

		// Process the animation nodes
		ProcessAnimationNodes(RootAnimNodeList);

		//Notify delegates
		OnPostProcessAnimationNodes.Broadcast(AllSubGraphsAnimNodeList, CompilerAccess, ClassAccess);
	}
	else
	{
		MessageLog.Error(*LOCTEXT("ExpectedAFunctionEntry_Error", "Expected at least one animation node, but did not find any").ToString());
	}
}

void FPaperZDAnimBPCompilerContext::ExpandSplitPins(UEdGraph* Graph)
{
	//ExpandSplitPins can change the size of the array, so we're better of using an iterator instead
	for (auto NodeIt = Graph->Nodes.CreateIterator(); NodeIt; ++NodeIt)
	{
		UK2Node* K2Node = Cast<UK2Node>(*NodeIt);
		if (K2Node != nullptr)
		{
			K2Node->ExpandSplitPins(*this, Graph);
		}
	}
}

void FPaperZDAnimBPCompilerContext::ProcessAnimationNodes(TArray<UPaperZDAnimGraphNode_Base*>& AnimNodeList)
{
	for (UPaperZDAnimGraphNode_Base* AnimNode : AnimNodeList)
	{
		ProcessAnimationNode(AnimNode);
	}
}

void FPaperZDAnimBPCompilerContext::ProcessAnimationNode(UPaperZDAnimGraphNode_Base* VisualAnimNode)
{
	// Early out if this node has already been processed
	if (AllocatedAnimNodes.Contains(VisualAnimNode))
	{
		return;
	}

	// Make sure the visual node has a runtime node template (FNode)
	const UScriptStruct* NodeType = VisualAnimNode->GetFNodeType();
	if (NodeType == NULL)
	{
		MessageLog.Error(TEXT("@@ has no animation node member"), VisualAnimNode);
		return;
	}

	// Give the visual node a chance to do validation
	{
		const int32 PreValidationErrorCount = MessageLog.NumErrors;
		VisualAnimNode->ValidateAnimNodeDuringCompilation(AnimBP, MessageLog);
		VisualAnimNode->BakeDataDuringCompilation(MessageLog);
		if (MessageLog.NumErrors != PreValidationErrorCount)
		{
			return;
		}
	}

	// Create a property for the node
	const FString NodeVariableName = ClassScopeNetNameMap.MakeValidName(VisualAnimNode);
	const UPaperZDAnimGraphSchema* AnimGraphDefaultSchema = GetDefault<UPaperZDAnimGraphSchema>();

	//Create the variable type descriptor 
	FEdGraphPinType NodeVariableType;
	NodeVariableType.PinCategory = UPaperZDAnimGraphSchema::PC_Struct;
	NodeVariableType.PinSubCategoryObject = MakeWeakObjectPtr(const_cast<UScriptStruct*>(NodeType));

	//Create a variable to describe the current node (of type "struct" as the FNode is)
	FStructProperty* NewProperty = CastField<FStructProperty>(CreateVariable(FName(*NodeVariableName), NodeVariableType));
	if (NewProperty == NULL)
	{
		MessageLog.Error(TEXT("Failed to create node property for @@"), VisualAnimNode);
	}

	// Register this node with the compile-time data structures
	const int32 AllocatedIndex = AllocateNodeIndexCounter++;
	AllocatedAnimNodes.Add(VisualAnimNode, NewProperty);
	AllocatedNodePropertiesToNodes.Add(NewProperty, VisualAnimNode);
	AllocatedAnimNodeIndices.Add(VisualAnimNode, AllocatedIndex);
	AllocatedPropertiesByIndex.Add(AllocatedIndex, NewProperty);

	UPaperZDAnimGraphNode_Base* TrueSourceObject = MessageLog.FindSourceObjectTypeChecked<UPaperZDAnimGraphNode_Base>(VisualAnimNode);
	SourceNodeToProcessedNodeMap.Add(TrueSourceObject, VisualAnimNode);

	// Register the slightly more permanent debug information
	//@TODO: Add this
 	//FPaperZDAnimBPDebugData& NewAnimBlueprintDebugData = NewAnimBlueprintClass->GetAnimBlueprintDebugData();
// 	NewAnimBlueprintDebugData.NodePropertyToIndexMap.Add(TrueSourceObject, AllocatedIndex);
// 	NewAnimBlueprintDebugData.NodeGuidToIndexMap.Add(TrueSourceObject->NodeGuid, AllocatedIndex);
// 	NewAnimBlueprintDebugData.NodePropertyIndexToNodeMap.Add(AllocatedIndex, TrueSourceObject);
// 	NewAnimBlueprintClass->GetDebugData().RegisterClassPropertyAssociation(TrueSourceObject, NewProperty);

	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	VisualAnimNode->ProcessDuringCompilation(CompilerAccess, ClassAccess);
}

void FPaperZDAnimBPCompilerContext::CopyTermDefaultsToDefaultObject(UObject* DefaultObject)
{
	FKismetCompilerContext::CopyTermDefaultsToDefaultObject(DefaultObject);

	//Bake the data onto the AnimInstance
	UPaperZDAnimInstance* AnimInstance = Cast<UPaperZDAnimInstance>(DefaultObject);
	if (AnimInstance)
	{
		int32 LinkIndexCount = 0;
		TMap<UPaperZDAnimGraphNode_Base*, int32> LinkIndexMap;
		TMap<UPaperZDAnimGraphNode_Base*, uint8*> NodeBaseAddresses;

		// We iterate on the class variables, to make sure that every variable we're setting has been setup on the class
		// variables that didn't get to the class (nodes), failed for some reason and they could still live on the maps
		// we also add everything to the "LinkIndexMap" which double checks the node existence in the posterior linking process
		//
		// NOTE: The field iterator always returns the data in the same order, the IDs are ordered such as properties will get the same
		// order when being "linked" on the class at runtime
		for (TFieldIterator<FProperty> It(AnimInstance->GetClass(), EFieldIteratorFlags::ExcludeSuper); It; ++It)
		{
			FProperty* TargetProperty = *It;
			UPaperZDAnimGraphNode_Base* AnimGraphNode = AllocatedNodePropertiesToNodes.FindRef(TargetProperty);

			//We should never have null pointers here, but checking to be safe
			if (AnimGraphNode && TargetProperty)
			{
				const FStructProperty* SourceNodeProperty = AnimGraphNode->GetFNodeProperty();
				check(SourceNodeProperty != NULL);
				check(CastFieldChecked<FStructProperty>(TargetProperty)->Struct == SourceNodeProperty->Struct);

				uint8* SourcePtr = SourceNodeProperty->ContainerPtrToValuePtr<uint8>(AnimGraphNode);
				uint8* DestinationPtr = TargetProperty->ContainerPtrToValuePtr<uint8>(AnimInstance);

				//Special handling
				if (UPaperZDAnimGraphNode_Sink* RootNode = ExactCast<UPaperZDAnimGraphNode_Sink>(AnimGraphNode))
				{
					// patch graph name into root nodes
					FPaperZDAnimNode_Sink NewRoot = *reinterpret_cast<FPaperZDAnimNode_Sink*>(SourcePtr);
					NewRoot.Name = Cast<UPaperZDAnimGraphNode_Sink>(MessageLog.FindSourceObject(RootNode))->GetGraph()->GetFName();
					TargetProperty->CopyCompleteValue(DestinationPtr, &NewRoot);
				}
				else
				{
					TargetProperty->CopyCompleteValue(DestinationPtr, SourcePtr);
				}

				LinkIndexMap.Add(AnimGraphNode, LinkIndexCount);
				NodeBaseAddresses.Add(AnimGraphNode, DestinationPtr);
				++LinkIndexCount;
			}
		}

		//Finally wire up animation data links
		for (const FPaperZDAnimDataLinkRecord& LinkRecord : LinkRecords)
		{
			UPaperZDAnimGraphNode_Base* LinkingNode = LinkRecord.GetLinkingNode();
			UPaperZDAnimGraphNode_Base* LinkedNode = LinkRecord.GetLinkedNode();

			//This check if to be doubly certain that the nodes we're linking got past the compilation process into the class
			if (LinkIndexMap.Contains(LinkingNode) && LinkIndexMap.Contains(LinkedNode))
			{
				const int32 SourceNodeIdx = LinkIndexMap.FindChecked(LinkingNode);
				const int32 LinkedNodeIdx = LinkIndexMap.FindChecked(LinkedNode);
				uint8* DestinationPtr = NodeBaseAddresses.FindChecked(LinkingNode);

				LinkRecord.PatchLinkIndex(DestinationPtr, LinkedNodeIdx, SourceNodeIdx);
			}
		}
	}

	//Call delegates
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	OnCopyTermDefaultsToDefaultObject.Broadcast(DefaultObject, CompilerAccess, ClassAccess);
}

void FPaperZDAnimBPCompilerContext::PreCompile()
{
	//Let the AnimBP know that it's gonna be compiled, so it prepares its graphs
	AnimBP->OnPreCompile();
}

void FPaperZDAnimBPCompilerContext::PostCompile()
{
	//@TODO: SHould we purge? this has issues if the sequence hasn't been loaded yet
	//Make sure the AnimSequences are up-to-date and don't contain any invalid notify data
// 	for (auto Seq : AnimBP->GetAnimSequences())
// 	{
// 		Seq->PurgeInvalidCustomNotifies(AnimBP->RegisteredNotifyNames);
// 	}

	//Let the AnimBP know that we finished compilation
	AnimBP->OnPostCompile();
}

void FPaperZDAnimBPCompilerContext::EnsureProperGeneratedClass(UClass*& InTargetClass)
{
	if (InTargetClass && !((UObject*)InTargetClass)->IsA(UPaperZDAnimBPGeneratedClass::StaticClass()))
	{
		FKismetCompilerUtilities::ConsignToOblivion(InTargetClass, Blueprint->bIsRegeneratingOnLoad);

		//Patch: This class is no longer valid and will be purged, but re-instancing code requires the class CDO which is nulled when the class is purged.
		//The re-instancer will then request the creation of a new CDO which triggers an ensure due to the class' layout
		//changing (the class flag never gets cleared out due to the class being dead).
		//Manually clearing the flag removes this behavior and has no consequences due to the class no longer being used anyway.
		InTargetClass->bLayoutChanging = false;
		InTargetClass = nullptr;
	}
}

void FPaperZDAnimBPCompilerContext::CleanAndSanitizeClass(UBlueprintGeneratedClass* ClassToClean, UObject*& InOldCDO)
{
	FKismetCompilerContext::CleanAndSanitizeClass(ClassToClean, InOldCDO);

	//Should be using the same pointers
	check(ClassToClean == NewClass && NewAnimBlueprintClass == NewClass);

	//Setup the supported animation class, so any node can check against it
	NewAnimBlueprintClass->SupportedAnimationSource = AnimBP->SupportedAnimationSource;

	//Call delegates
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	OnStartCompilingClass.Broadcast(NewAnimBlueprintClass, CompilerAccess, ClassAccess);
}

void FPaperZDAnimBPCompilerContext::FinishCompilingClass(UClass* Class)
{
	FKismetCompilerContext::FinishCompilingClass(Class);

	//Call delegates
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	OnFinishCompilingClass.Broadcast(NewAnimBlueprintClass, CompilerAccess, ClassAccess);
}

bool FPaperZDAnimBPCompilerContext::ShouldForceKeepNode(const UEdGraphNode* Node) const
{
	// Force keep anim nodes during the standard pruning step. Isolated ones will then be removed via PruneIsolatedAnimationNodes.
	// Anim graph nodes are always culled at their expansion step anyways.
	return Node->IsA<UPaperZDAnimGraphNode_Base>();
}

void FPaperZDAnimBPCompilerContext::PostExpansionStep(const UEdGraph* Graph)
{
	FKismetCompilerContext::PostExpansionStep(Graph);

	// Cull out all anim nodes as they dont contribute to execution at all
	for (int32 NodeIndex = 0; NodeIndex < ConsolidatedEventGraph->Nodes.Num(); ++NodeIndex)
	{
		if (UPaperZDAnimGraphNode_Base* Node = Cast<UPaperZDAnimGraphNode_Base>(ConsolidatedEventGraph->Nodes[NodeIndex]))
		{
			Node->BreakAllNodeLinks();
			ConsolidatedEventGraph->Nodes.RemoveAtSwap(NodeIndex);
			--NodeIndex;
		}
	}

	//Call delegates
	FPaperZDAnimBPCompilerAccess CompilerAccess(this);
	FPaperZDAnimBPGeneratedClassAccess ClassAccess(NewAnimBlueprintClass);
	OnPostExpansionStep.Broadcast(Graph, CompilerAccess, ClassAccess);
}

#undef LOCTEXT_NAMESPACE