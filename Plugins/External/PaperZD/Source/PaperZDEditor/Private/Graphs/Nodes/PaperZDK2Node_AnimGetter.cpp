// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDK2Node_AnimGetter.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_PlaySequence.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_State.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/PaperZDStateMachineGraph.h"
#include "Graphs/PaperZDAnimTransitionGraphSchema.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "PaperZDAnimBP.h"
#include "PaperZDAnimInstance.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDK2Node_AnimGetter)
#endif

#define LOCTEXT_NAMESPACE "PaperZDTransitionGraphNode_Result"

void UPaperZDK2Node_AnimGetter::PostPasteNode()
{
	Super::PostPasteNode();

	//Apply any patch up to the state machine nodes
	RestoreStateMachineNode();
}

void UPaperZDK2Node_AnimGetter::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// TODO: Find a nicer way to maybe pull these down from the instance class and allow 
	// projects to add new parameters from derived instances
	TArray<FString> PinNames;
	PinNames.Add(TEXT("CurrentTime"));
	PinNames.Add(TEXT("AssetPlayerIndex"));
	PinNames.Add(TEXT("MachineIndex"));
 	PinNames.Add(TEXT("StateIndex"));
// 	PinNames.Add(TEXT("TransitionIndex"));

	for (FString& PinName : PinNames)
	{
		if (UEdGraphPin* FoundPin = FindPin(PinName))
		{
			FoundPin->bHidden = true;
		}
	}
}

FText UPaperZDK2Node_AnimGetter::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return CachedTitle;
}

bool UPaperZDK2Node_AnimGetter::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return Cast<UPaperZDAnimTransitionGraphSchema>(Schema) != nullptr || Cast<UPaperZDAnimGraphSchema>(Schema) != nullptr;
}

bool UPaperZDK2Node_AnimGetter::IsActionFilteredOut(FBlueprintActionFilter const& Filter)
{
	if (Filter.Context.Graphs.Num() > 0)
	{
		UEdGraph* CurrGraph = Filter.Context.Graphs[0];

		if (CurrGraph && Filter.Context.Blueprints.Num() > 0)
		{
			UPaperZDAnimBP* AnimBlueprint = Cast<UPaperZDAnimBP>(Filter.Context.Blueprints[0]);
			check(AnimBlueprint);

			if (SourceAnimBlueprint == AnimBlueprint)
			{
				// Get the native anim instance derived class
				UClass* NativeInstanceClass = AnimBlueprint->ParentClass;
				while (NativeInstanceClass && !NativeInstanceClass->HasAnyClassFlags(CLASS_Native))
				{
					NativeInstanceClass = NativeInstanceClass->GetSuperClass();
				}

				if (GetterClass != NativeInstanceClass)
				{
					// If the anim instance containing the getter is not the class we're currently using then bail
					return true;
				}

				const UEdGraphSchema* Schema = CurrGraph->GetSchema();

				// Bail if we aren't meant for this graph at all
				if (!IsContextValidForSchema(Schema))
				{
					return true;
				}

				if (Cast<UPaperZDAnimTransitionGraphSchema>(Schema))
				{
					if (!SourceNode && !SourceStateNode)
					{
						// No dependancies, always allow
						return false;
					}

					// Inside a transition graph
					if (SourceNode)
					{
						if (UPaperZDStateGraphNode_Transition* TransitionNode = Cast<UPaperZDStateGraphNode_Transition>(CurrGraph->GetOuter()))
						{
							if (SourceStateNode)
							{
								if (UPaperZDStateGraphNode_State* PreviousStateNode = Cast<UPaperZDStateGraphNode_State>(TransitionNode->GetFromNode()))
								{
									// Only allow actions using states that are referencing the previous state
									if (SourceStateNode == PreviousStateNode)
									{
										return false;
									}
								}
							}
							else if (UPaperZDAnimGraphNode_StateMachine* MachineNode = Cast<UPaperZDAnimGraphNode_StateMachine>(SourceNode))
							{
								// Available everywhere
								return false;
							}
							else if (UPaperZDStateGraphNode_State* PrevStateNode = Cast<UPaperZDStateGraphNode_State>(TransitionNode->GetFromNode()))
							{
								// Make sure the attached asset node is in the source graph
								if (SourceNode && SourceNode->GetGraph() == PrevStateNode->GetBoundGraph())
								{
									return false;
								}
							}
						}
					}
				}
				else if (Cast<UPaperZDAnimGraphSchema>(Schema))
				{
					// Inside normal anim graph
					if (SourceStateNode)
					{
						for (UBlueprint* Blueprint : Filter.Context.Blueprints)
						{
							TArray<UPaperZDStateGraphNode_State*> StateNodes;
							FBlueprintEditorUtils::GetAllNodesOfClass(Blueprint, StateNodes);

							if (StateNodes.Contains(SourceStateNode))
							{
								return false;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void UPaperZDK2Node_AnimGetter::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// First cache the available functions for getters
	UClass* ActionKey = GetClass();
	const UPaperZDAnimBP* AnimBlueprint = Cast<UPaperZDAnimBP>(ActionRegistrar.GetActionKeyFilter());
	if (AnimBlueprint && ActionRegistrar.IsOpenForRegistration(AnimBlueprint))
	{
		//In theory we know that we only derive natively from the AnimInstance, but in case we have inheritance later this is safer
		UClass* BPClass = *AnimBlueprint->ParentClass;
		while (BPClass && !BPClass->HasAnyClassFlags(CLASS_Native))
		{
			BPClass = BPClass->GetSuperClass();
		}

		if (BPClass)
		{
			TArray<UFunction*> AnimGetters;
			for (TFieldIterator<UFunction> FuncIter(BPClass); FuncIter; ++FuncIter)
			{
				UFunction* Func = *FuncIter;

				if (Func->HasMetaData(TEXT("AnimGetter")) && Func->HasAnyFunctionFlags(FUNC_Native))
				{
					AnimGetters.Add(Func);
				}
			}

			auto UiSpecOverride = [](const FBlueprintActionContext& /*Context*/, const IBlueprintNodeBinder::FBindingSet& Bindings, FBlueprintActionUiSpec* UiSpecOut, FText Title)
			{
				UiSpecOut->MenuName = Title;
			};

			TArray<UPaperZDAnimGraphNode_PlaySequence*> AssetPlayerNodes;
			TArray<UPaperZDAnimGraphNode_StateMachine*> MachineNodes;
			TArray<UPaperZDStateGraphNode_State*> StateNodes;			

			FBlueprintEditorUtils::GetAllNodesOfClass(AnimBlueprint, AssetPlayerNodes);
			FBlueprintEditorUtils::GetAllNodesOfClass(AnimBlueprint, MachineNodes); 
			FBlueprintEditorUtils::GetAllNodesOfClass(AnimBlueprint, StateNodes);

			for (UFunction* Getter : AnimGetters)
			{
				FNodeSpawnData Params;
				Params.AnimInstanceClass = BPClass;
				Params.Getter = Getter;
				Params.SourceBlueprint = AnimBlueprint;
				Params.GetterContextString = Getter->GetMetaData(TEXT("GetterContext"));

				if (GetterRequiresParameter(Getter, TEXT("AssetPlayerIndex")))
				{
					for (UPaperZDAnimGraphNode_PlaySequence* PlayerNode : AssetPlayerNodes)
					{
						// Should always succeed
						if (UPaperZDAnimSequence* AnimSequence = PlayerNode->GetAnimSequence())
						{
							FText Title = FText::Format(LOCTEXT("NodeTitle", "{0} ({1})"), Getter->GetDisplayNameText(), FText::FromString(*AnimSequence->GetName()));
							Params.SourceNode = PlayerNode;
							Params.CachedTitle = Title;

							UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(UPaperZDK2Node_AnimGetter::StaticClass(), /*AssetNode->GetGraph()*/nullptr, UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateUObject(const_cast<UPaperZDK2Node_AnimGetter*>(this), &UPaperZDK2Node_AnimGetter::PostSpawnNodeSetup, Params));
							Spawner->DynamicUiSignatureGetter = UBlueprintNodeSpawner::FUiSpecOverrideDelegate::CreateStatic(UiSpecOverride, Title);
							ActionRegistrar.AddBlueprintAction(AnimBlueprint, Spawner);
						}
					}
				}
				else if (GetterRequiresParameter(Getter, TEXT("MachineIndex")))
				{
					if (GetterRequiresParameter(Getter, TEXT("StateIndex")))
					{
						for (UPaperZDStateGraphNode_State* StateNode : StateNodes)
						{
							// Get the state machine node from the outer chain
							UPaperZDStateMachineGraph* Graph = Cast<UPaperZDStateMachineGraph>(StateNode->GetOuter());
							if (Graph)
							{
								if (UPaperZDAnimGraphNode_StateMachine* MachineNode = Cast<UPaperZDAnimGraphNode_StateMachine>(Graph->GetOuter()))
								{
									Params.SourceNode = MachineNode;
								}
							}

							FText Title = FText::Format(LOCTEXT("NodeTitle", "{0} ({1})"), Getter->GetDisplayNameText(), StateNode->GetNodeTitle(ENodeTitleType::ListView));
							Params.SourceStateNode = StateNode;
							Params.CachedTitle = Title;

							UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(UPaperZDK2Node_AnimGetter::StaticClass(), /*StateNode->GetGraph()*/nullptr, UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateUObject(const_cast<UPaperZDK2Node_AnimGetter*>(this), &UPaperZDK2Node_AnimGetter::PostSpawnNodeSetup, Params));
							Spawner->DynamicUiSignatureGetter = UBlueprintNodeSpawner::FUiSpecOverrideDelegate::CreateStatic(UiSpecOverride, Title);
							ActionRegistrar.AddBlueprintAction(AnimBlueprint, Spawner);
						}
					}
					//@NOTE: Possibly add here transition index, if we have something to check (like running transitional state)
					else
					{
						// Only requires the state machine
						for (UPaperZDAnimGraphNode_StateMachine* MachineNode : MachineNodes)
						{
							FText Title = FText::Format(LOCTEXT("NodeTitle", "{0} ({1})"), Getter->GetDisplayNameText(), MachineNode->GetNodeTitle(ENodeTitleType::ListView));
							Params.SourceNode = MachineNode;
							Params.CachedTitle = Title;

							UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(UPaperZDK2Node_AnimGetter::StaticClass(), /*MachineNode*/nullptr, UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateUObject(const_cast<UPaperZDK2Node_AnimGetter*>(this), &UPaperZDK2Node_AnimGetter::PostSpawnNodeSetup, Params));
							Spawner->DynamicUiSignatureGetter = UBlueprintNodeSpawner::FUiSpecOverrideDelegate::CreateStatic(UiSpecOverride, Title);
							ActionRegistrar.AddBlueprintAction(AnimBlueprint, Spawner);
						}
					}
				}
				else
				{
					// Doesn't operate on a node, only need one entry
					FText Title = FText::Format(LOCTEXT("NodeTitleNoNode", "{0}"), Getter->GetDisplayNameText());
					Params.CachedTitle = Title;

					UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(UPaperZDK2Node_AnimGetter::StaticClass(), nullptr, UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateUObject(const_cast<UPaperZDK2Node_AnimGetter*>(this), &UPaperZDK2Node_AnimGetter::PostSpawnNodeSetup, Params));
					Spawner->DynamicUiSignatureGetter = UBlueprintNodeSpawner::FUiSpecOverrideDelegate::CreateStatic(UiSpecOverride, Title);
					ActionRegistrar.AddBlueprintAction(AnimBlueprint, Spawner);
				}
			}
		}
	}
}

void UPaperZDK2Node_AnimGetter::RestoreStateMachineNode()
{
	if (SourceStateNode)
	{
		UPaperZDStateMachineGraph* Graph = Cast<UPaperZDStateMachineGraph>(SourceStateNode->GetOuter());
		if (Graph)
		{
			if (UPaperZDAnimGraphNode_StateMachine* MachineNode = Cast<UPaperZDAnimGraphNode_StateMachine>(Graph->GetOuter()))
			{
				SourceNode = MachineNode;
			}
		}
	}
}

bool UPaperZDK2Node_AnimGetter::GetterRequiresParameter(const UFunction* Getter, FString ParamName) const
{
	bool bRequiresParameter = false;

	for (TFieldIterator<FProperty> PropIt(Getter); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
	{
		FProperty* Prop = *PropIt;

		if (Prop->GetName() == ParamName)
		{
			bRequiresParameter = true;
			break;
		}
	}

	return bRequiresParameter;
}

bool UPaperZDK2Node_AnimGetter::IsContextValidForSchema(const UEdGraphSchema* Schema) const
{
	//No filtering for now
	return true;
}

void UPaperZDK2Node_AnimGetter::PostSpawnNodeSetup(UEdGraphNode* NewNode, bool bIsTemplateNode, FNodeSpawnData SpawnData)
{
	UPaperZDK2Node_AnimGetter* TypedNode = CastChecked<UPaperZDK2Node_AnimGetter>(NewNode);

	// Apply parameters
	TypedNode->SourceNode = SpawnData.SourceNode;
	TypedNode->SourceStateNode = SpawnData.SourceStateNode;
	TypedNode->GetterClass = SpawnData.AnimInstanceClass;
	TypedNode->SourceAnimBlueprint = SpawnData.SourceBlueprint;
	TypedNode->SetFromFunction((UFunction*)SpawnData.Getter);
	TypedNode->CachedTitle = SpawnData.CachedTitle;

	SpawnData.GetterContextString.ParseIntoArray(TypedNode->Contexts, TEXT("|"), 1);
}

#undef LOCTEXT_NAMESPACE
