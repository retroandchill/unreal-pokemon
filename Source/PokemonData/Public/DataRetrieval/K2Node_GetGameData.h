// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_GetGameData.generated.h"

/**
 * Node to handle getting the game data out of the editor
 */
UCLASS()
class POKEMONDATA_API UK2Node_GetGameData : public UK2Node {
	GENERATED_BODY()

public:
	/**
	 * Set up the node assigning the struct that this should be retrieving
	 * @param NodeStruct The struct type for this node
	 */
	void Initialize(UScriptStruct* NodeStruct);

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool IsNodePure() const override;

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override; 

private:
	/**
	 * The struct type represented by this node
	 */
	UPROPERTY()
	TObjectPtr<UScriptStruct> StructType;
};
