// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_PlaySequence.h"
#include "Graphs/PaperZDAnimGraph.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/ARFilter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/CompilerResultsLog.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_PlaySequence)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_PlaySequence::UPaperZDAnimGraphNode_PlaySequence()
	: Super()
{}

FString UPaperZDAnimGraphNode_PlaySequence::GetNodeCategory() const
{
	return TEXT("Animation");
}

void UPaperZDAnimGraphNode_PlaySequence::PreloadRequiredAssets()
{
	PreloadObject(AnimNode.AnimSequence);

	Super::PreloadRequiredAssets();
}

FLinearColor UPaperZDAnimGraphNode_PlaySequence::GetNodeTitleColor() const
{
	return FColor(200, 100, 100);
}

FText UPaperZDAnimGraphNode_PlaySequence::GetTooltipText() const
{
	if (AnimNode.AnimSequence == nullptr)
	{
		return FText();
	}

	return GetNodeTooltipBySequencePath(AnimNode.AnimSequence->GetPathName());
}

FText UPaperZDAnimGraphNode_PlaySequence::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (AnimNode.AnimSequence == nullptr)
	{
		return LOCTEXT("PlaySequence_Title_NoSequence", "Play Sequence");
	}

	return GetNodeTitleBySequenceName(AnimNode.AnimSequence->GetFName());
}

FText UPaperZDAnimGraphNode_PlaySequence::GetNodeTitleBySequenceName(FName SequenceName) const
{
	//Construct the path
	FFormatNamedArguments Args;
	FText Name = FText::FromName(SequenceName);
	Args.Add(TEXT("Name"), Name);
	return FText::Format(LOCTEXT("PlaySequence_Title", "Play {Name}"), Args);
}

FText UPaperZDAnimGraphNode_PlaySequence::GetNodeTooltipBySequencePath(FString SequencePath) const
{
	//Construct the path
	FFormatNamedArguments Args;
	FText Path = FText::FromString(SequencePath);
	Args.Add(TEXT("Path"), Path);
	return FText::Format(LOCTEXT("PlaySequence_Tooltip", "Play {Path}"), Args);
}

void UPaperZDAnimGraphNode_PlaySequence::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	auto LoadedAssetSetup = [](UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UPaperZDAnimSequence> SequencePtr)
	{
		UPaperZDAnimGraphNode_PlaySequence* GraphNode = CastChecked<UPaperZDAnimGraphNode_PlaySequence>(NewNode);
		GraphNode->AnimNode.AnimSequence = SequencePtr.Get();
	};

	auto UnloadedAssetSetup = [](UEdGraphNode* NewNode, bool bIsTemplateNode, const FAssetData AssetData)
	{
		UPaperZDAnimGraphNode_PlaySequence* GraphNode = CastChecked<UPaperZDAnimGraphNode_PlaySequence>(NewNode);
		if (!bIsTemplateNode)
		{
			UPaperZDAnimSequence* Sequence = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());
			check(Sequence != nullptr);
			GraphNode->AnimNode.AnimSequence = Sequence;
		}
		else
		{
			//The asset is not loaded, but we can get the Tag value that represents the AnimBP
			AssetData.GetTagValue(UPaperZDAnimSequence::GetAnimSourceMemberName(), GraphNode->PROTO_AnimSourceTag);
		}
	};
		
	const UObject* QueryObject = ActionRegistrar.GetActionKeyFilter();
	if (QueryObject == nullptr)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		// define a filter to help in pulling the AnimSequence asset data from the registry
		FARFilter Filter;
		Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
		Filter.bRecursiveClasses = true;
		// Find matching assets and add an entry for each one
		TArray<FAssetData> SequenceList;
		AssetRegistryModule.Get().GetAssets(Filter, /*out*/SequenceList);

		//Register ~every~ AnimSequence as an action that can possibly be added to the graph (will be filtered out in later stages)
		for (const FAssetData& Asset : SequenceList)
		{
			UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
			if (Asset.IsAssetLoaded())
			{
				UPaperZDAnimSequence* Sequence = Cast<UPaperZDAnimSequence>(Asset.GetAsset());
				if (Sequence)
				{
					TWeakObjectPtr<UPaperZDAnimSequence> SequencePtr = MakeWeakObjectPtr(const_cast<UPaperZDAnimSequence*>(Sequence));
					NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(LoadedAssetSetup, SequencePtr);
					NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Sequence->GetFName());
					NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Sequence->GetPathName());

					ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
				}
			}
			else
			{
				NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(UnloadedAssetSetup, Asset);
				NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Asset.AssetName);
				NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Asset.GetObjectPathString());

				ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
			}
		}
	}
	else if (const UPaperZDAnimSequence* Sequence = Cast<const UPaperZDAnimSequence>(QueryObject))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());

		TWeakObjectPtr<UPaperZDAnimSequence> SequencePtr = MakeWeakObjectPtr(const_cast<UPaperZDAnimSequence*>(Sequence));
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(LoadedAssetSetup, SequencePtr);
		NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Sequence->GetFName());
		NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Sequence->GetPathName());

		ActionRegistrar.AddBlueprintAction(QueryObject, NodeSpawner);
	}
	else if (QueryObject == GetClass())
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		// define a filter to help in pulling the AnimSequence asset data from the registry
		FARFilter Filter;
		Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
		Filter.bRecursiveClasses = true;
		// Find matching assets and add an entry for each one
		TArray<FAssetData> SequenceList;
		AssetRegistryModule.Get().GetAssets(Filter, /*out*/SequenceList);

		//Register ~every~ AnimSequence as an action that can possibly be added to the graph (will be filtered out in later stages)
		for (const FAssetData& Asset : SequenceList)
		{
			if (!Asset.IsAssetLoaded())
			{
				UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
				NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(UnloadedAssetSetup, Asset);
				NodeSpawner->DefaultMenuSignature.MenuName = GetNodeTitleBySequenceName(Asset.AssetName);
				NodeSpawner->DefaultMenuSignature.Tooltip = GetNodeTooltipBySequencePath(Asset.GetObjectPathString());

				ActionRegistrar.AddBlueprintAction(Asset, NodeSpawner);
			}
		}
	}
}

bool UPaperZDAnimGraphNode_PlaySequence::IsActionFilteredOut(class FBlueprintActionFilter const& Filter)
{
	//We can only be used contextually by animations that share the same animation source
	if (AnimNode.AnimSequence)
	{
		for (UBlueprint* Blueprint : Filter.Context.Blueprints)
		{
			UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Blueprint);
			if (!AnimBP || AnimBP->GetSupportedAnimationSource() != AnimNode.AnimSequence->GetAnimSource())
			{
				return true;
			}
		}
	}
	else if (!PROTO_AnimSourceTag.IsEmpty())
	{
		//Asset is unloaded, we will instead check against the value that was filled on the PROTO node
		for (UBlueprint* Blueprint : Filter.Context.Blueprints)
		{
			UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Blueprint);
			UPaperZDAnimationSource* AnimSource = AnimBP ? AnimBP->GetSupportedAnimationSource() : nullptr;
			if (!AnimSource || FAssetData(AnimSource).GetExportTextName() != PROTO_AnimSourceTag)
			{
				return true;
			}
		}
	}
	else
	{
		//NULL-node, had no AnimSequence info and wasn't a PROTO node
		return true;
	}

	//Node passed all verifications, can be used
	return false;
}

void UPaperZDAnimGraphNode_PlaySequence::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();

	//Transitional anim graph requires this node to be forced as non-looping
	if (GetAnimGraph()->IsTransitionalGraph())
	{
		AnimNode.bLoopAnimation = false;
	}
}

void UPaperZDAnimGraphNode_PlaySequence::PostPasteNode()
{
	Super::PostPasteNode();

	//Transitional anim graph requires this node to be forced as non-looping
	if (GetAnimGraph()->IsTransitionalGraph())
	{
		//Pin should be hidden first, as the default value will be copied into the property when hidden
		SetShowPinForProperty(GET_MEMBER_NAME_CHECKED(FPaperZDAnimNode_PlaySequence, bLoopAnimation), false);
		AnimNode.bLoopAnimation = false;
	}
}

void UPaperZDAnimGraphNode_PlaySequence::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	Super::ValidateAnimNodeDuringCompilation(ForAnimBP, MessageLog);

	//We should always have an AnimSequence linked to us
	UPaperZDAnimSequence* CheckedSequence = AnimNode.AnimSequence;
	UEdGraphPin* SequencePin = FindPin(GET_MEMBER_NAME_STRING_CHECKED(FPaperZDAnimNode_PlaySequence, AnimSequence));
	if (SequencePin != nullptr && CheckedSequence == nullptr)
	{
		//No Sequence on the AnimNode, the pin could have it added
		CheckedSequence = Cast<UPaperZDAnimSequence>(SequencePin->DefaultObject);
	}

	//If the sequence is null, then we might have the node bound to it
	if (CheckedSequence == nullptr)
	{
		// we may have a connected node or binding
		if (SequencePin == nullptr || (SequencePin->LinkedTo.Num() == 0))
		{
			MessageLog.Error(TEXT("@@ references an unknown sequence"), this);
		}
	}
	else
	{
		//If we do have a sequence, we should make sure that it is compatible to the AnimBP that we are on
		if (CheckedSequence->GetAnimSource() != ForAnimBP->GetSupportedAnimationSource())
		{
			MessageLog.Error(TEXT("@@ references sequence that is not compatible with the current AnimBP"), this);
		}

		//If this node is inside a transitional anim graph, we cannot loop the animation
		UPaperZDAnimGraph* AnimGraph = Cast<UPaperZDAnimGraph>(GetGraph());
		if (AnimGraph && AnimGraph->IsTransitionalGraph() && AnimNode.bLoopAnimation)
		{
			MessageLog.Warning(TEXT("@@ is set to looping on a transitional anim graph, this is not supported. Will compile as non-looping animation."), this);
			AnimNode.bLoopAnimation = false;
		}
	}

}

#undef LOCTEXT_NAMESPACE