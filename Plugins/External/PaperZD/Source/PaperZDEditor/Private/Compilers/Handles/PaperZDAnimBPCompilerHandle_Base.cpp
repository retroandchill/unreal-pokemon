// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Compilers/Handles/PaperZDAnimBPCompilerHandle_Base.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "Compilers/Access/PaperZDAnimBPGeneratedClassAccess.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "PaperZDAnimInstance.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_CallArrayFunction.h"
#include "K2Node_StructMemberSet.h"
#include "K2Node_StructMemberGet.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet/KismetArrayLibrary.h"

//////////////////////////////////////////////////////////////////////////
//FEvaluationHandlerRecord
//////////////////////////////////////////////////////////////////////////
void FPaperZDAnimBPCompilerHandle_Base::FEvaluationHandlerRecord::RegisterPin(UEdGraphPin* DestPin, FProperty* AssociatedProperty, int32 AssociatedPropertyArrayIndex)
{
	FAnimNodeSinglePropertyHandler& Handler = ServicedProperties.FindOrAdd(AssociatedProperty->GetFName());
	TArray<FString> DestPropertyPath;

	// Prepend the destination property with the node's member property if the property is not on a UClass
	if (Cast<UClass>(AssociatedProperty->Owner.ToUObject()) == nullptr)
	{
		DestPropertyPath.Add(NodeVariableProperty->GetName());
	}

	if (AssociatedPropertyArrayIndex != INDEX_NONE)
	{
		DestPropertyPath.Add(FString::Printf(TEXT("%s[%d]"), *AssociatedProperty->GetName(), AssociatedPropertyArrayIndex));
	}
	else
	{
		DestPropertyPath.Add(AssociatedProperty->GetName());
	}

	//Create a copy-record for the given pin
	Handler.CopyRecords.Emplace(DestPin, AssociatedProperty, AssociatedPropertyArrayIndex, MoveTemp(DestPropertyPath));
}

void FPaperZDAnimBPCompilerHandle_Base::FEvaluationHandlerRecord::SetupExposedHandler(FPaperZDExposedValueHandler& Handler) const
{
	//No FAST-PATH support, so we only copy the function name and its node property
	Handler.ValueHandlerNodeProperty = NodeVariableProperty;
	Handler.BoundFunction = HandlerFunctionName;
}

//////////////////////////////////////////////////////////////////////////
//FEvaluationHandlerRecord
//////////////////////////////////////////////////////////////////////////
bool FPaperZDAnimBPCompilerHandle_Base::FEffectiveConstantRecord::Apply(UObject* Object)
{
	uint8* PropertyPtr = nullptr;

	// Check the node property is a member of the object's class
	check(NodeVariableProperty->GetOwner<UClass>() && Object->GetClass()->IsChildOf(NodeVariableProperty->GetOwner<UClass>()));
	// Check the constant property is a member of the node's struct
	check(ConstantProperty->GetOwner<UStruct>() && NodeVariableProperty->Struct->IsChildOf(ConstantProperty->GetOwner<UStruct>()));

	//Get the value pointers
	uint8* StructPtr = NodeVariableProperty->ContainerPtrToValuePtr<uint8>(Object); //Pointer to the FNode structure instance
	PropertyPtr = ConstantProperty->ContainerPtrToValuePtr<uint8>(StructPtr); //Pointer to the property inside the FNode structure instance

	//Copy the data into the CDO
	if (ArrayIndex != INDEX_NONE)
	{
		FArrayProperty* ArrayProperty = CastFieldChecked<FArrayProperty>(ConstantProperty);
		FScriptArrayHelper ArrayHelper(ArrayProperty, PropertyPtr);

		if (ArrayHelper.IsValidIndex(ArrayIndex))
		{
			FBlueprintEditorUtils::PropertyValueFromString_Direct(ArrayProperty->Inner, LiteralSourcePin->GetDefaultAsString(), ArrayHelper.GetRawPtr(ArrayIndex));
		}
		else
		{
			return false;
		}
	}
	else
	{
		FBlueprintEditorUtils::PropertyValueFromString_Direct(ConstantProperty, LiteralSourcePin->GetDefaultAsString(), PropertyPtr);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//FPaperZDAnimBPCompilerHandle_Base
//////////////////////////////////////////////////////////////////////////
void FPaperZDAnimBPCompilerHandle_Base::Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess)
{
	InCompilerAccess.OnFinishCompilingClass().AddRaw(this, &FPaperZDAnimBPCompilerHandle_Base::FinishCompilingClass);
	InCompilerAccess.OnCopyTermDefaultsToDefaultObject().AddRaw(this, &FPaperZDAnimBPCompilerHandle_Base::CopyTermDefaultsToDefaultObject);
}

void FPaperZDAnimBPCompilerHandle_Base::AddStructEvalHandlers(UPaperZDAnimGraphNode_Base* InNode, FPaperZDAnimBPCompilerAccess& InCompilerContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	const UPaperZDAnimGraphSchema* AnimGraphDefaultSchema = GetDefault<UPaperZDAnimGraphSchema>();
	FEvaluationHandlerRecord& EvalHandler = PerNodeStructEvalHandlers.Add(InNode);

	//Find the generated variable that corresponds to this node
	FStructProperty* NodeProperty = CastFieldChecked<FStructProperty>(InCompilerContext.GetAllocatedPropertyFromNode(InNode));
	
	//Iterate on the available node input pins and process their data
	for (auto SourcePinIt = InNode->Pins.CreateIterator(); SourcePinIt; ++SourcePinIt)
	{
		UEdGraphPin* SourcePin = *SourcePinIt;
		if (SourcePin->Direction == EGPD_Input)
		{
			bool bConsumed = false;

			//If this node is an input and an Animation link, we should store this for later patch-up
			if (AnimGraphDefaultSchema->IsAnimationDataPin(SourcePin->PinType))
			{
				FPaperZDAnimDataLinkRecord LinkRecord = InNode->GetLinkIDLocation(NodeProperty->Struct, SourcePin);
				if (LinkRecord.IsValid())
				{
					InCompilerContext.AddAnimDataLinkRecord(LinkRecord);
					bConsumed = true;
				}
			}
			else
			{
				//Check the value that is bound to this pin
				FProperty* SourcePinProperty = nullptr;
				int32 SourceArrayIndex = INDEX_NONE;
				InNode->GetPinAssociatedProperty(NodeProperty->Struct, SourcePin, SourcePinProperty, SourceArrayIndex);

				//Pin is bound to a property
				if (SourcePinProperty != nullptr)
				{
					if (SourcePin->LinkedTo.Num() == 0)
					{
						//The pin is not bound to anything. This means that we should use the default value of the pin and paste it on the CDO as a constant property
						new (ValidAnimNodePinConstants) FEffectiveConstantRecord(NodeProperty, SourcePin, SourcePinProperty, SourceArrayIndex);
					}
					else
					{
						//Pin is bound to another node, cannot be assumed as a constant record
						ensure(EvalHandler.NodeVariableProperty == nullptr || EvalHandler.NodeVariableProperty == NodeProperty);
						EvalHandler.AnimGraphNode = InNode;
						EvalHandler.NodeVariableProperty = NodeProperty;
						EvalHandler.RegisterPin(SourcePin, SourcePinProperty, SourceArrayIndex);
					}

					//Pin officially consumed
					bConsumed = true;

					//Register debug data
	// 				UEdGraphPin* TrueSourcePin = InCompilationContext.GetMessageLog().FindSourcePin(SourcePin);
	// 				if (TrueSourcePin)
	// 				{
	// 					OutCompiledData.GetBlueprintDebugData().RegisterClassPropertyAssociation(TrueSourcePin, SourcePinProperty);
	// 				}
				}
			}

			//Pin got ignored for some reason (no property bound to it?)
			if (!bConsumed)
			{
				InCompilerContext.GetMessageLog().Note(TEXT("@@ was visible but ignored"), SourcePin);
			}
		}
	}
	
	//Next, any property bindings we need to add
	//@NOTE: Property binding support requires "PropertyAccess" module and special code. ~~PENDING~~
}

void FPaperZDAnimBPCompilerHandle_Base::CreateEvaluationHandlerForNode(FPaperZDAnimBPCompilerAccess& InCompilationContext, UPaperZDAnimGraphNode_Base* InNode)
{
	if (FEvaluationHandlerRecord* RecordPtr = PerNodeStructEvalHandlers.Find(InNode))
	{
		// Generate a new event to update the value of these properties
		FEvaluationHandlerRecord& Record = *RecordPtr;
		
		if (Record.NodeVariableProperty)
		{
			CreateEvaluationHandler(InCompilationContext, InNode, Record);

			int32 NewIndex = ValidEvaluationHandlerList.Add(Record);
			ValidEvaluationHandlerMap.Add(InNode, NewIndex);
		}
	}
}

void FPaperZDAnimBPCompilerHandle_Base::CreateEvaluationHandler(FPaperZDAnimBPCompilerAccess& InCompilationContext, UPaperZDAnimGraphNode_Base* InNode, FEvaluationHandlerRecord& Record)
{
	// Shouldn't create a handler if there is nothing to work with
	check(Record.ServicedProperties.Num() > 0);
	check(Record.NodeVariableProperty != NULL);

	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	const UPaperZDAnimGraphSchema* AnimGraphDefaultSchema = GetDefault<UPaperZDAnimGraphSchema>();

	// Use the node GUID for a stable name across compiles
	FString FunctionName = FString::Printf(TEXT("%s_%s_%s_%s"), *AnimGraphDefaultSchema->GetDefaultEvaluationHandlerName().ToString(), *InNode->GetOuter()->GetName(), *InNode->GetClass()->GetName(), *InNode->NodeGuid.ToString());
	Record.HandlerFunctionName = FName(*FunctionName);

	// check function name isnt already used (data exists that can contain duplicate GUIDs) and apply a numeric extension until it is unique
	int32 ExtensionIndex = 0;
	FName* ExistingName = HandlerFunctionNames.Find(Record.HandlerFunctionName);
	while (ExistingName != nullptr)
	{
		FunctionName = FString::Printf(TEXT("%s_%s_%s_%s_%d"), *AnimGraphDefaultSchema->GetDefaultEvaluationHandlerName().ToString(), *InNode->GetOuter()->GetName(), *InNode->GetClass()->GetName(), *InNode->NodeGuid.ToString(), ExtensionIndex);
		Record.HandlerFunctionName = FName(*FunctionName);
		ExistingName = HandlerFunctionNames.Find(Record.HandlerFunctionName);
		ExtensionIndex++;
	}

	//Store the handler function name for later use
	HandlerFunctionNames.Add(Record.HandlerFunctionName);

	//
	//The system will now create a custom event that basically boils down to the following
	// ## CustomEvent -> Set Members on Struct (FAnimNode) ##
	// Because the internal node is always a struct, this event/function will effectively update the data on the AnimNode
	// The entry points to each section of the "set member" will be wired into the nodes that connect to the input of this GraphNode
	//
	// TL;DR: the "AnimGraphNode" becomes a StructMembersSet node in this step.
	//

	// Add a custom event in the graph (entry point)
	UK2Node_CustomEvent* CustomEventNode = InCompilationContext.SpawnIntermediateNode<UK2Node_CustomEvent>(InNode, InCompilationContext.GetConsolidatedEventGraph());
	CustomEventNode->bInternalEvent = true;
	CustomEventNode->CustomFunctionName = Record.HandlerFunctionName;
	CustomEventNode->AllocateDefaultPins();
	Record.CustomEventNodes.Add(CustomEventNode);

	// The ExecChain is the current exec output pin in the linear chain
	UEdGraphPin* ExecChain = K2Schema->FindExecutionPin(*CustomEventNode, EGPD_Output);

	//Begin creating the node structure and wiring up
	//Create the Member Set node
	UK2Node_StructMemberSet* AssignmentNode = InCompilationContext.SpawnIntermediateNode<UK2Node_StructMemberSet>(InNode, InCompilationContext.GetConsolidatedEventGraph());
	AssignmentNode->VariableReference.SetSelfMember(Record.NodeVariableProperty->GetFName());
	AssignmentNode->StructType = Record.NodeVariableProperty->Struct;
	AssignmentNode->AllocateDefaultPins();
	Record.CustomEventNodes.Add(AssignmentNode);

	UEdGraphPin* ExecVariablesIn = K2Schema->FindExecutionPin(*AssignmentNode, EGPD_Input);
	ExecChain->MakeLinkTo(ExecVariablesIn);
	ExecChain = K2Schema->FindExecutionPin(*AssignmentNode, EGPD_Output);

	// Run thru each property
	TSet<FName> PropertiesBeingSet;
	for (UEdGraphPin* TargetPin : AssignmentNode->Pins)
	{
		FName PropertyName(TargetPin->PinName);

		//If it gets serviced by this handler, bind
		if (FAnimNodeSinglePropertyHandler* SourceInfo = Record.ServicedProperties.Find(PropertyName))
		{
			if (TargetPin->PinType.IsArray())
			{
				//Create a getter ONLY for the array variable
				UK2Node_StructMemberGet* ArrayGetter = InCompilationContext.SpawnIntermediateNode<UK2Node_StructMemberGet>(InNode, InCompilationContext.GetConsolidatedEventGraph());
				ArrayGetter->VariableReference.SetSelfMember(Record.NodeVariableProperty->GetFName());
				ArrayGetter->StructType = Record.NodeVariableProperty->Struct;
				ArrayGetter->AllocatePinsForSingleMemberGet(TargetPin->PinName);
				Record.CustomEventNodes.Add(ArrayGetter);

				//Grab the array variable pin
				UEdGraphPin* ArrayVariableNode = ArrayGetter->FindPin(PropertyName);

				// Set each element in the array that has a record
				for (FPropertyCopyRecord& CopyRecord : SourceInfo->CopyRecords)
				{
					int32 ArrayIndex = CopyRecord.DestArrayIndex;
					if (UEdGraphPin* DestPin = CopyRecord.DestPin)
					{
						//The array SETTER
						UK2Node_CallArrayFunction* ArraySet = InCompilationContext.SpawnIntermediateNode<UK2Node_CallArrayFunction>(InNode, InCompilationContext.GetConsolidatedEventGraph());
						ArraySet->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UKismetArrayLibrary, Array_Set), UKismetArrayLibrary::StaticClass());
						ArraySet->AllocateDefaultPins();
						Record.CustomEventNodes.Add(ArraySet);

						// Connect the execution chain
						ExecChain->MakeLinkTo(ArraySet->GetExecPin());
						ExecChain = ArraySet->GetThenPin();

						// Connect the input array
						UEdGraphPin* TargetArrayPin = ArraySet->FindPinChecked(TEXT("TargetArray"));
						TargetArrayPin->MakeLinkTo(ArrayVariableNode);
						ArraySet->PinConnectionListChanged(TargetArrayPin);

						// Set the array index
						UEdGraphPin* TargetIndexPin = ArraySet->FindPinChecked(TEXT("Index"));
						TargetIndexPin->DefaultValue = FString::FromInt(ArrayIndex);

						// Wire up the data input
						UEdGraphPin* TargetItemPin = ArraySet->FindPinChecked(TEXT("Item"));
						TargetItemPin->CopyPersistentDataFromOldPin(*DestPin);
						InCompilationContext.GetMessageLog().NotifyIntermediatePinCreation(TargetItemPin, DestPin);
					}

					// ~~
					// The data now looks like this
					// StructMemberSet -> SetArrayValue1 -> SetArrayValue2...
					// Array Properties are not set on the STRUCT SETTER, so we won't add them to that node that way the struct setter doesn't override them
					// ~~
				}
			}
			else
			{
				//Single property, bind directly (the following check should always work)
				if (SourceInfo->CopyRecords.Num() > 0 && SourceInfo->CopyRecords[0].DestPin != nullptr)
				{
					UEdGraphPin* DestPin = SourceInfo->CopyRecords[0].DestPin;
					PropertiesBeingSet.Add(DestPin->PinName);
					TargetPin->CopyPersistentDataFromOldPin(*DestPin);
					InCompilationContext.GetMessageLog().NotifyIntermediatePinCreation(TargetPin, DestPin);
				}
			}
		}
	}

	//Pins that haven't been bound are "hidden" and are being treated as a property to be set on the CDO (via the Property Window)
	//The Struct Set node will still try to bind them and will "overwrite" the values we have set, so we must remove any unbound pin so that doesn't happen.
	for (FOptionalPinFromProperty& OptionalPinInfo : AssignmentNode->ShowPinForProperties)
	{
		OptionalPinInfo.bShowPin = PropertiesBeingSet.Contains(OptionalPinInfo.PropertyName);
	}

	//Node might have changed (hidden properties)... reconstruct
	AssignmentNode->ReconstructNode();
}

void FPaperZDAnimBPCompilerHandle_Base::FinishCompilingClass(const UClass* InClass, FPaperZDAnimBPCompilerAccess& InCompilerContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	// Without the property access system we need to patch generated function names here
	// Create one exposed handler per HandlerRecord (pass the function information onto the CLASS)
	TArray<FPaperZDExposedValueHandler>& ExposedValueHandlers = OutCompiledData.GetExposedValueHandlers();
	for (FEvaluationHandlerRecord& HandlerRecord : ValidEvaluationHandlerList)
	{
		HandlerRecord.EvaluationHandlerIdx = ExposedValueHandlers.Num();
		FPaperZDExposedValueHandler& ExposedValueHandler = ExposedValueHandlers.AddDefaulted_GetRef();
		HandlerRecord.SetupExposedHandler(ExposedValueHandler);
	}
}

void FPaperZDAnimBPCompilerHandle_Base::CopyTermDefaultsToDefaultObject(UObject* InDefaultObject, FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	UPaperZDAnimInstance* AnimInstance = Cast<UPaperZDAnimInstance>(InDefaultObject);
	if (AnimInstance)
	{
		//Apply the constant records first (pins not bound to anything that need to be pasted onto the CDO)
		for (FEffectiveConstantRecord& Record : ValidAnimNodePinConstants)
		{
			//Try to apply the node's default value
			if (Record.Apply(AnimInstance) == false)
			{
				InCompilationContext.GetMessageLog().Error(TEXT("ICE: Failed to push literal value from @@ into CDO"), Record.LiteralSourcePin);
			}
		}

		//@TODO: When fast path is available, we should mark the nodes depending on their blueprint usage.
	}
}
