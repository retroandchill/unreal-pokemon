// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_CacheAnimation.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "Kismet2/Kismet2NameValidators.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_CacheAnimation)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

/////////////////////////////////////////////////////
// FCachedAnimationNameValidator

class FCachedAnimationNameValidator : public FStringSetNameValidator
{
public:
	FCachedAnimationNameValidator(class UBlueprint* InBlueprint, const FString& InExistingName)
		: FStringSetNameValidator(InExistingName)
	{
		TArray<UPaperZDAnimGraphNode_CacheAnimation*> Nodes;
		FBlueprintEditorUtils::GetAllNodesOfClass<UPaperZDAnimGraphNode_CacheAnimation>(InBlueprint, Nodes);

		for (auto Node : Nodes)
		{
			Names.Add(Node->GetCacheName());
		}
	}
};

/////////////////////////////////////////////////////
// UPaperZDAnimGraphNode_CacheAnimation

UPaperZDAnimGraphNode_CacheAnimation::UPaperZDAnimGraphNode_CacheAnimation()
	: Super()
{
	bCanRenameNode = true;
}

FString UPaperZDAnimGraphNode_CacheAnimation::GetNodeCategory() const
{
	return TEXT("Cached Animations");
}

FText UPaperZDAnimGraphNode_CacheAnimation::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::EditableTitle)
	{
		return FText::FromString(CacheName);
	}
	else if ((TitleType == ENodeTitleType::MenuTitle) && CacheName.IsEmpty())
	{
		return LOCTEXT("NewCachedAnimation", "New Cached Animation...");
	}
	else if (CachedNodeTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), FText::FromString(CacheName));
		CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("ZDAnimGraphNode_CacheAnimation_Title", "Cache animation '{NodeTitle}'"), Args), this);
	}
	return CachedNodeTitle;
}

void UPaperZDAnimGraphNode_CacheAnimation::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	auto PostSpawnSetupLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode)
	{
		UPaperZDAnimGraphNode_CacheAnimation* CachedAnimationNode = CastChecked<UPaperZDAnimGraphNode_CacheAnimation>(NewNode);
		// we use an empty CacheName in GetNodeTitle() to relay the proper menu title
		if (!bIsTemplateNode)
		{
			// @TODO: is the idea that this name is unique? what if Rand() hit twice? why not MakeUniqueObjectName? (In that case, what would the Outer be?)
			CachedAnimationNode->CacheName = TEXT("CachedAnimation") + FString::FromInt(FMath::Rand());
		}
	};

	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(PostSpawnSetupLambda);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

bool UPaperZDAnimGraphNode_CacheAnimation::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	bool const bIsNotStateMachine = TargetGraph->GetOuter()->IsA(UPaperZDAnimBP::StaticClass());
	return bIsNotStateMachine && Super::IsCompatibleWithGraph(TargetGraph);
}

void UPaperZDAnimGraphNode_CacheAnimation::OnRenameNode(const FString& NewName)
{
	CacheName = NewName;
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

TSharedPtr<class INameValidatorInterface> UPaperZDAnimGraphNode_CacheAnimation::MakeNameValidator() const
{
	return MakeShareable(new FCachedAnimationNameValidator(GetBlueprint(), CacheName));
}

#undef LOCTEXT_NAMESPACE
