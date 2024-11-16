// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node_CallFunction.h"
#include "BlueprintActionFilter.h"
#include "PaperZDK2Node_AnimGetter.generated.h"

class UPaperZDAnimGraphNode_Base;
class UPaperZDStateGraphNode_State;
class UPaperZDAnimBP;

/**
 * Re-implementation of K2Node_AnimGetter that works with PaperZD's AnimInstances and Animation Graphs.
 */
UCLASS(MinimalAPI)
class UPaperZDK2Node_AnimGetter : public UK2Node_CallFunction
{
	GENERATED_BODY()

	/* Spawn data package to be passed when setting up the node*/
	struct FNodeSpawnData
	{
		//ctor
		FNodeSpawnData()
			: SourceNode(nullptr)
			, SourceStateNode(nullptr)
			, AnimInstanceClass(nullptr)
			, SourceBlueprint(nullptr)
			, Getter(nullptr)
		{}

		// Title to use for the spawned node
		FText CachedTitle;

		// The node the spawned getter accesses
		TObjectPtr<UPaperZDAnimGraphNode_Base> SourceNode;

		// The state node the spawned getter accesses
		TObjectPtr<UPaperZDStateGraphNode_State> SourceStateNode;

		// The instance class the spawned getter is defined on
		TObjectPtr<UClass> AnimInstanceClass;

		// The blueprint the getter is valid within
		TObjectPtr<const UPaperZDAnimBP> SourceBlueprint;

		// The UFunction (as a UField) 
		TObjectPtr<UField> Getter;

		// String of combined valid contexts for the spawned getter
		FString GetterContextString;
	};

public:

	// Anim node required for the getter
	UPROPERTY()
	TObjectPtr<UPaperZDAnimGraphNode_Base> SourceNode;

	// Optional state node that is required for the getter
	UPROPERTY()
	TObjectPtr<UPaperZDStateGraphNode_State> SourceStateNode;

	// AnimInstance class that implements the getter
	UPROPERTY()
	TObjectPtr<UClass> GetterClass;

	// The anim blueprint that generated this getter
	UPROPERTY()
	TObjectPtr<const UPaperZDAnimBP> SourceAnimBlueprint;

	// Cached node title
	UPROPERTY()
	FText CachedTitle;

	// List of valid contexts for the node
	UPROPERTY()
	TArray<FString> Contexts;

public:

	// UObject interface
	virtual void PostPasteNode() override;
	// End of UObject interface

	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual bool IsActionFilteredOut(FBlueprintActionFilter const& Filter) override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// end of UK2Node interface

private:
	// Fixes the SourceNode to be the state machine owner of SourceStateNode (if it is not null)
	void RestoreStateMachineNode();

	/** Returns whether or not the provided UFunction requires the named parameter */
	bool GetterRequiresParameter(const UFunction* Getter, FString ParamName) const;

	/** Checks the cached context strings to make sure this getter is valid within the provided schema */
	bool IsContextValidForSchema(const UEdGraphSchema* Schema) const;

 	/** Passed to blueprint spawners to configure spawned nodes */
 	void PostSpawnNodeSetup(UEdGraphNode* NewNode, bool bIsTemplateNode, FNodeSpawnData SpawnData);
};
