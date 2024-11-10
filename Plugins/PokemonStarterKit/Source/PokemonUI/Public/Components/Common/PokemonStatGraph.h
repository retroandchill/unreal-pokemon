// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Widgets/SCompoundWidget.h"

class SConstraintCanvas;
class SCanvas;
class IPokemon;

/**
 * @class SPokemonStatGraph
 *
 * @brief A class to represent and manage a Pokémon's stat graph.
 *
 * SPokemonStatGraph handles the visualization and management of
 * a Pokémon's various statistics such as health, attack, defense, etc.
 * The class provides mechanisms to update and display these statistics
 * in a graphical format.
 *
 * Main functionalities include setting stat values, updating the graph,
 * and drawing the stat graph on the screen.
 */
class POKEMONUI_API SPokemonStatGraph : public SCompoundWidget {
  public:
    SLATE_BEGIN_ARGS(SPokemonStatGraph) : _DefaultAngleOffset(0.f), _MinimumDesiredSize(FVector2D(250)) {
        }

        SLATE_ARGUMENT(TArray<FMainStatHandle>, StatNames)
        SLATE_ATTRIBUTE(float, DefaultAngleOffset)
        SLATE_ARGUMENT(FLinearColor, GridLinesColor)
        SLATE_ARGUMENT(FLinearColor, NodeLinesColor)
        SLATE_ATTRIBUTE(FVector2D, MinimumDesiredSize)
        SLATE_END_ARGS()

        /**
         * @brief Constructs the SPokemonStatGraph widget with the specified arguments.
         *
         * This method initializes the stat graph widget using the provided arguments,
         * setting up the various display and configuration parameters such as stat names,
         * angle offset, grid line color, and node line color.
         *
         * @param InArgs The arguments to use for constructing the widget. This includes:
         * - TArray<FMainStatHandle> StatNames: The names of the stats to be displayed.
         * - float DefaultAngleOffset: The default angle offset for the stat graph.
         * - FLinearColor GridLinesColor: The color to use for the grid lines.
         * - FLinearColor NodeLinesColor: The color to use for the node lines.
         * - FVector2D MinimumDesiredSize: The minimum desired size of the stat graph.
         */
        void Construct(const FArguments &InArgs);

        int32 OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect,
                      FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle,
                      bool bParentEnabled) const override;
        void Tick(const FGeometry &AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
        FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

        /**
         * @brief Retrieves a pointer to the Pokémon interface.
         *
         * This method returns a constant reference to a TWeakInterfacePtr
         * object, which is a weak pointer to an IPokemon interface. It is
         * used to access the Pokémon associated with this stat graph
         * without taking ownership of the object.
         *
         * @return A constant reference to a TWeakInterfacePtr object representing
         *         a weak pointer to the IPokemon interface.
         */
        const TWeakInterfacePtr<IPokemon> &GetPokemon() const {
            return Pokemon;
        }

        /**
         * @brief Sets the current Pokémon.
         *
         * This method assigns the provided Pokémon object to the internal
         * Pokémon reference, allowing the class to manage and manipulate
         * the specified Pokémon.
         *
         * @param InPokemon The Pokémon object to be set.
         */
        void SetPokemon(const TScriptInterface<IPokemon> &InPokemon) {
            Pokemon = InPokemon;
        }

        /**
         * @brief Retrieves the color used for the grid lines in the stat graph.
         *
         * This method returns a reference to the FLinearColor object that defines the color
         * used for drawing the grid lines in the Pokémon stat graph. This color is set during
         * the construction of the SPokemonStatGraph widget and can be modified later using
         * the SetGridLinesColor method.
         *
         * @return A reference to the FLinearColor object representing the grid lines color.
         */
        const FLinearColor &GetGridLinesColor() const {
            return GridLinesColor;
        }

        /**
         * @brief Sets the color for the grid lines in the stat graph.
         *
         * This method updates the color used for drawing the grid lines in the
         * Pokémon stat graph. The new color is provided as a parameter and is
         * applied immediately.
         *
         * @param InGridLinesColor The new color to be used for the grid lines.
         */
        void SetGridLinesColor(const FLinearColor &InGridLinesColor) {
            GridLinesColor = InGridLinesColor;
        }

        /**
         * @brief Retrieves the color used for the node lines in the stat graph.
         *
         * This method returns a reference to the FLinearColor object that defines the color
         * used for drawing the node lines in the Pokémon stat graph. This color is set during
         * the construction of the SPokemonStatGraph widget and can be modified later using
         * the SetNodeLinesColor method.
         *
         * @return A reference to the FLinearColor object representing the node lines color.
         */
        const FLinearColor &GetNodeLinesColor() const {
            return NodeLinesColor;
        }

        /**
         * @brief Sets the color for the node lines in the stat graph.
         *
         * Updates the color used for drawing the node lines in the Pokémon stat graph.
         * The provided color parameter is applied immediately to reflect the changes.
         *
         * @param InNodeLinesColor The new color to be used for the node lines.
         */
        void SetNodeLinesColor(const FLinearColor &InNodeLinesColor) {
            NodeLinesColor = InNodeLinesColor;
        }

        /**
         * @brief Retrieves the minimum desired size of the stat graph.
         *
         * This method returns a reference to a TAttribute object containing
         * the minimum desired size for the SPokemonStatGraph widget.
         *
         * @return A reference to the TAttribute<FVector2D> object representing
         * the minimum desired size of the stat graph.
         */
        const TAttribute<FVector2D> &GetMinimumDesiredSize() const {
            return MinimumDesiredSize;
        }

        /**
         * @brief Sets the minimum desired size for the stat graph.
         *
         * This method updates the minimum desired size of the SPokemonStatGraph widget.
         * The new size is specified using an attribute containing a FVector2D value.
         *
         * @param InMinimumDesiredSize The attribute containing the new minimum desired size
         *        for the stat graph.
         */
        void SetMinimumDesiredSize(const TAttribute<FVector2D> &InMinimumDesiredSize) {
            MinimumDesiredSize = InMinimumDesiredSize;
        }

      private:
        void CacheNodeData(const FGeometry &AllottedGeometry);
        static FVector2D FromNormalToSlateSpace(const FVector2D NormalSpace, const FGeometry &AllottedGeometry);

        void PaintLines(const FGeometry &AllottedGeometry, FSlateWindowElementList &OutDrawElements,
                        int32 &DrawLayerId) const;
        TWeakInterfacePtr<IPokemon> Pokemon;
        TArray<FMainStatHandle> StatNames;
        TArray<FVector2D> CachedNodeLocations;

        TAttribute<float> AngleOffsetAttribute;
        float DefaultAngleOffset = 0.f;

        FLinearColor GridLinesColor;
        FLinearColor NodeLinesColor;
        TAttribute<FVector2D> MinimumDesiredSize;
    };