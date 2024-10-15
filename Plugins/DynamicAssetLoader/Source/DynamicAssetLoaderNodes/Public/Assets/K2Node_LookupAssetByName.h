// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_LookupAssetByName.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICASSETLOADERNODES_API UK2Node_LookupAssetByName : public UK2Node {
    GENERATED_BODY()

public:
    UK2Node_LookupAssetByName();
    void Initialize(FName InAssetKey);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    void ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;

    bool IsNodeSafeToIgnore() const override {
        return true;
    }

    void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
    FText GetMenuCategory() const override;

    /** Get the then output pin */
    UEdGraphPin *GetAssetFoundPin() const;
    /** Get the spawn transform input pin */
    UEdGraphPin *GetAssetNamePin() const;
    /** Get the exec output pin for when the row was not found */
    UEdGraphPin *GetAssetNotFoundPin() const;
    /** Get the result output pin */
    UEdGraphPin *GetResultPin() const;

    /** Get the type of the TableRow to return */
    UClass *GetAssetClassType() const;

private:
    void SetPinToolTip(UEdGraphPin &MutatablePin, const FText &PinDescription) const;

    UPROPERTY()
    FName AssetKey;

    /** Tooltip text for this node. */
    FText NodeTooltip;

    /** Constructing FText strings can be costly, so we cache the node's title */
    FNodeTextCache CachedNodeTitle;

};
