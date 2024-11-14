// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_UseCachedAnimation.h"
#include "PaperZDAnimGraphNode_UseCachedAnimation.generated.h"

class UPaperZDAnimGraphNode_CacheAnimation;

/**
 * Reads from a cached animation node.
 */
UCLASS()
class UPaperZDAnimGraphNode_UseCachedAnimation : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* friendship for compilation purposes. */
	friend class FPaperZDAnimBPCompilerHandle_CacheAnimation;

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_UseCachedAnimation AnimNode;

	/* Name of the cache node we're linked with. */
	UPROPERTY()
	mutable FString NameOfCache;

	/* The CacheAnimation node we're using. */
	UPROPERTY()
	mutable TWeakObjectPtr<UPaperZDAnimGraphNode_CacheAnimation> LinkedCacheNode;

public:
	UPaperZDAnimGraphNode_UseCachedAnimation();

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	virtual void OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
	//~End UPaperZDAnimGraphNode_Base Interface

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// End of UEdGraphNode interface

	// UK2Node interface.
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
	// End of UK2Node interface
};
