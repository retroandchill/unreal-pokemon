﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Switch.h"
#include "K2Node_SwitchName.h"
#include "DataRetrieval/DataStructHandle.h"

#include "K2Node_SwitchOnDataHandle.generated.h"

struct FStatusHandle;
/**
 * 
 */
UCLASS()
class POKEMONBLUEPRINTNODES_API UK2Node_SwitchOnDataHandle : public UK2Node_Switch {
    GENERATED_BODY()

public:
    explicit UK2Node_SwitchOnDataHandle(const FObjectInitializer& ObjectInitializer);
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    void PostLoad() override;

    FText GetTooltipText() const override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    bool ShouldShowNodeProperties() const override;

    void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

    void AddPinToSwitchNode() override;
    FName GetUniquePinName() override;
    FEdGraphPinType GetPinType() const override;
    FEdGraphPinType GetInnerCaseType() const override;

protected:
    void CreateFunctionPin() override;
    void CreateSelectionPin() override;
    void CreateCasePins() override;
    void RemovePin(UEdGraphPin* TargetPin) override;
    
private:
    UPROPERTY(EditAnywhere, Category = PinOptions)
    TArray<FStatusHandle> PinHandles;

    UPROPERTY()
    TArray<FName> PinNames;
};
