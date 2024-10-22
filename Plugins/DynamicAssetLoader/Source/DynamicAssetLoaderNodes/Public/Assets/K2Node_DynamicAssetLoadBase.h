// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "Ranges/Utilities/Methods.h"
#include "K2Node_DynamicAssetLoadBase.generated.h"

struct FAssetClassType;

/**
 * @class UK2Node_DynamicAssetLoadBase
 * Base class for dynamic asset loading nodes.
 *
 * This class is part of the K2 (Blueprint) system in Unreal Engine, enabling
 * the dynamic loading of assets during runtime. It serves as a foundational
 * node that other dynamic asset loading nodes can extend.
 */
UCLASS(Abstract)
class DYNAMICASSETLOADERNODES_API UK2Node_DynamicAssetLoadBase : public UK2Node {
    GENERATED_BODY()

public:
    UK2Node_DynamicAssetLoadBase();

    /**
     * Initializes the node with the specified asset key.
     *
     * @param InAssetKey The key of the asset to be set for this node.
     */
    void Initialize(FName InAssetKey);

    void AllocateDefaultPins() override;
    FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    FText GetTooltipText() const override;
    void ExpandNode(class FKismetCompilerContext &CompilerContext, UEdGraph *SourceGraph) override;
    FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
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

protected:
    /**
     * Sets the tooltip text for the node.
     *
     * @param Text The tooltip text to set.
     */
    template <typename T>
        requires std::convertible_to<std::remove_cvref_t<T>, FText>
    void SetNodeTooltip(T&& Text) {
        NodeTooltip = Forward<T>(Text);
    }

    /**
     * Sets the tooltip for a specified pin.
     *
     * @param MutatablePin The pin for which the tooltip will be set.
     * @param PinDescription The description text to be added to the pin's tooltip.
     */
    void SetPinToolTip(UEdGraphPin &MutatablePin, const FText &PinDescription) const;

    /**
     * Get the title format of the node.
     *
     * @return The text format for the node's title.
     */
    virtual FText GetNodeTitleFormat() const ABSTRACT_METHOD;

    /**
     * Creates the results pin for the specified asset class.
     *
     * @param AssetClass The class type of the asset for which the results pin is to be created.
     * @return A pointer to the created results pin.
     */
    virtual UEdGraphPin* CreateResultsPin(const FAssetClassType& AssetClass) ABSTRACT_METHOD;

    /**
     * Gets the name of the function to load the asset.
     *
     * @return The name of the load function.
     */
    virtual FName GetLoadFunctionName() const ABSTRACT_METHOD;

private:
    UPROPERTY()
    FName AssetKey;

    /** Tooltip text for this node. */
    FText NodeTooltip;

    /** Constructing FText strings can be costly, so we cache the node's title */
    FNodeTextCache CachedNodeTitle;

};
