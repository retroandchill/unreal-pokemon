// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonInfoWidget.h"

#include "PokemonStatDisplay.generated.h"

class UPokemonStatEntry;
struct FMainStatHandle;
class SPokemonStatGraph;

/**
 * @class UPokemonStatDisplay
 * @brief A widget class that displays Pokémon stats in a graphical format.
 *
 * This class is responsible for rendering the stats of a Pokémon in a graphical manner,
 * depicting various stats arranged in a specified order. The appearance including colors
 * and sizes can be customized.
 */
UCLASS()
class POKEMONUI_API UPokemonStatDisplay : public UWidget
{
    GENERATED_BODY()

  public:
    TSharedRef<SWidget> RebuildWidget() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;
    void SynchronizeProperties() override;

    /**
     * @brief Retrieves the color used for the inner grid lines in the stat display widget.
     *
     * This function returns a reference to the FLinearColor that specifies the color for
     * the inner lines within the Pokémon stats graphical display. It is used to style
     * and enhance the visual differentiation within the stat grid.
     *
     * @return A reference to the FLinearColor object representing the color of the inner lines.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const FLinearColor &GetInnerLinesColor() const
    {
        return InnerLinesColor;
    }

    /**
     * @brief Sets the color for the inner grid lines in the stat display widget.
     *
     * Updates the color used to render the inner lines within the Pokémon stats graphical display.
     * This enhances the visual differentiation and adds styling to the stat grid.
     * The new color is applied and the properties of the widget are synchronized.
     *
     * @param InInnerLinesColor The new color to be applied to the inner grid lines.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetInnerLinesColor(const FLinearColor &InInnerLinesColor);

    /**
     * @brief Retrieves the color used for the outline in the stat display widget.
     *
     * This function returns a reference to the FLinearColor that specifies the color for
     * the outline of the Pokémon stats graphical display. It is used to style
     * and enhance the visual differentiation of the stat graph's boundary.
     *
     * @return A reference to the FLinearColor object representing the color of the outline.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const FLinearColor &GetOutlineColor() const
    {
        return OutlineColor;
    }

    /**
     * @brief Sets the outline color for the stat display widget.
     *
     * This function updates the color used to render the outer lines forming
     * the boundary of the Pokémon stats graphical display. The new color is applied
     * and the properties of the widget are synchronized.
     *
     * @param InOutlineColor The new color to be applied to the outline.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetOutlineColor(const FLinearColor &InOutlineColor);

    /**
     * @brief Retrieves the minimum desired size for the widget.
     *
     * This method returns the minimum dimensions (width and height) that the widget
     * should occupy. It can be used to ensure that the widget is displayed with sufficient
     * space allocated to it.
     *
     * @return A constant reference to a FVector2D representing the minimum desired size.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const FVector2D &GetMinimumDesiredSize() const
    {
        return MinimumDesiredSize;
    }

    /**
     * @brief Sets the minimum desired size for the Pokémon stat display widget.
     *
     * This method updates the minimum dimensions that the widget should adhere to
     * when being rendered. The size is specified as a 2D vector indicating width
     * and height.
     *
     * @param InMinimumDesiredSize The new minimum size for the widget, represented as an FVector2D.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetMinimumDesiredSize(const FVector2D &InMinimumDesiredSize);

    /**
     * @brief Retrieves the associated Pokémon interface.
     *
     * This method returns the interface representing the Pokémon linked with this object.
     * It is purposed for internal blueprint usage and ensures that the caller gets a constant
     * reference to the Pokémon instance.
     *
     * @return A constant reference to the TScriptInterface representing the Pokémon.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<IPokemon> &GetPokemon() const
    {
        return Pokemon;
    }

    /**
     * @fn UPokemonStatDisplay::SetPokemon
     * @brief Sets the current Pokémon to display its stats.
     *
     * This method assigns a new Pokémon whose stats will be rendered in the widget.
     * It updates the internal data and synchronizes the visual properties accordingly.
     *
     * @param NewPokemon The new Pokémon to be displayed, wrapped in a TScriptInterface.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetPokemon(const TScriptInterface<IPokemon> &NewPokemon);

  private:
    /**
     * The order of the stats that are being drawn starting from the top and going around clockwise.
     */
    UPROPERTY(EditAnywhere, Category = Content)
    TArray<FMainStatHandle> StatOrder;

    /**
     * @brief Specifies the color for inner grid lines in the stat display widget.
     *
     * This variable determines the color used to render the inner lines within the
     * Pokémon stats graphical display. It enhances visual differentiation and adds
     * styling to the stat grid.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetInnerLinesColor, BlueprintSetter = SetInnerLinesColor,
              Category = Style)
    FLinearColor InnerLinesColor = FLinearColor::Gray;

    /**
     * @brief Defines the color of the outline used in the Pokémon stats graphical display.
     *
     * This variable sets the color for the outer lines forming the boundary of the stat graph.
     * It helps to visually delineate the edges of the graphical representation of the Pokémon stats,
     * enhancing the overall styling and clarity.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetOutlineColor, BlueprintSetter = SetOutlineColor, Category = Style)
    FLinearColor OutlineColor = FLinearColor::White;

    /**
     * @brief Sets the minimum desired size for the stat display widget.
     *
     * This variable defines the minimum dimensions the Pokémon stat display widget
     * should be rendered at. Ensuring the widget respects this size helps to maintain
     * a consistent and visually appealing layout regardless of the container's size.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMinimumDesiredSize, BlueprintSetter = SetMinimumDesiredSize,
              Category = Layout)
    FVector2D MinimumDesiredSize = {250, 250};

    /**
     * @brief Interface reference to a Pokémon object used in the UI widget.
     *
     * This variable holds a reference to a Pokémon object that implements the IPokemon interface.
     * It is used in the UPokemonStatDisplay class to represent and manipulate the displayed Pokémon's
     * stats within the graphical widget. The reference can be accessed and modified through Blueprint
     * getter and setter functions.
     *
     * @see UPokemonStatDisplay::GetPokemon
     * @see UPokemonStatDisplay::SetPokemon
     */
    UPROPERTY(BlueprintGetter = GetPokemon, BlueprintSetter = SetPokemon, Category = Content)
    TScriptInterface<IPokemon> Pokemon;

    TSharedPtr<SPokemonStatGraph> StatGraph;
};
