// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Widgets/SCompoundWidget.h"

class IPokemon;
/**
 * 
 */
class POKEMONUI_API SPokemonStatGraph : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(SPokemonStatGraph) : _DefaultAngleOffset(0.f),
                                          _DrawRadius(0.9f) {
        }

        SLATE_ATTRIBUTE(TArray<FName>, StatNames)
        SLATE_ATTRIBUTE(float, DefaultAngleOffset)
        SLATE_ARGUMENT(float, DrawRadius)
        SLATE_ARGUMENT(FLinearColor, GridLinesColor)
        SLATE_ARGUMENT(FLinearColor, NodeLinesColor)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments &InArgs);

    int32 OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect,
                  FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle,
                  bool bParentEnabled) const override;

    void Tick(const FGeometry &AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    void CacheNodeData(const FGeometry &AllottedGeometry);
    static FVector2D FromNormalToSlateSpace(const FVector2D NormalSpace, const FGeometry &AllottedGeometry);

    void PaintLines(const FGeometry &AllottedGeometry, FSlateWindowElementList &OutDrawElements,
                    int32 &DrawLayerId) const;

    TWeakInterfacePtr<IPokemon> Pokemon;
    TAttribute<TArray<FName>> StatNames;
    TArray<FName> CachedStatNames;
    TArray<FVector2D> CachedNodeLocations;

    TAttribute<float> AngleOffsetAttribute;
    float DefaultAngleOffset = 0.f;
    float DrawRadius = 0.9f;

    FLinearColor GridLinesColor;
    FLinearColor NodeLinesColor;
};