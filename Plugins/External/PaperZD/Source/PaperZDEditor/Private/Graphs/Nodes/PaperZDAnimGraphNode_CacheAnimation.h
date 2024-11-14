// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_CacheAnimation.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "PaperZDAnimGraphNode_CacheAnimation.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

/**
 * Caches the animation data enabling it to be read from multiple nodes at a time, making sure the animation sub-tree is only updated and evaluated once per frame.
 * To access the data use the "Use Cached Animation" node.
 */
UCLASS()
class UPaperZDAnimGraphNode_CacheAnimation : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_CacheAnimation AnimNode;

	/* Name of the cache node. */
	UPROPERTY()
	FString CacheName;

	/* Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedNodeTitle;

public:
	UPaperZDAnimGraphNode_CacheAnimation();

	//getters
	FORCEINLINE FString GetCacheName() const { return CacheName; }

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	virtual bool RequiresOutputPin() const { return false; }
	//~End UPaperZDAnimGraphNode_Base Interface

	//~ Begin UEdGraphNode Interface.
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void OnRenameNode(const FString& NewName) override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	virtual bool IsNodeRootSet() const override { return true; }
	//~ End UEdGraphNode Interface.

	// UK2Node interface.
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of UK2Node interface
};
