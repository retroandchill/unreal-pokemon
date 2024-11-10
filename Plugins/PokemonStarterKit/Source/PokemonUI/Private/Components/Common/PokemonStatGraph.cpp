// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonStatGraph.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SPokemonStatGraph::Construct(const FArguments &InArgs) {
    StatNames = InArgs._StatNames;
    AngleOffsetAttribute = InArgs._DefaultAngleOffset;
    GridLinesColor = InArgs._GridLinesColor;
    NodeLinesColor = InArgs._NodeLinesColor;
    MinimumDesiredSize = InArgs._MinimumDesiredSize;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

int32 SPokemonStatGraph::OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry,
                                 const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements,
                                 int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const {
    PaintLines(AllottedGeometry, OutDrawElements, LayerId);
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
                                    bParentEnabled);
}

void SPokemonStatGraph::Tick(const FGeometry &AllottedGeometry, const double InCurrentTime, const float InDeltaTime) {
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    CacheNodeData(AllottedGeometry);
}

FVector2D SPokemonStatGraph::ComputeDesiredSize(float LayoutScaleMultiplier) const {
    return MinimumDesiredSize.Get();
}

void SPokemonStatGraph::CacheNodeData(const FGeometry &AllottedGeometry) {
    const int32 NumNodes = StatNames.Num();
    if (NumNodes < 3) {
        return;
    }

    CachedNodeLocations.Empty(NumNodes);

    // Add first node
    const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
    const float AngleSeparation = -360.0f / NumNodes;
    const FVector2D TopNormalized = FVector2D(0.0f, 1.f).GetRotated(AngleOffset);

    // Setup nodes in Geometry space
    for (int32 i = 0; i < NumNodes; i++) {
        const FVector2D NormNode = TopNormalized.GetRotated(AngleSeparation * i);
        CachedNodeLocations.Add(FromNormalToSlateSpace(NormNode, AllottedGeometry));
    }
}

FVector2D SPokemonStatGraph::FromNormalToSlateSpace(const FVector2D NormalSpace, const FGeometry &AllottedGeometry) {
    const FVector2D Center = AllottedGeometry.GetLocalSize() / 2.0f;
    const float GeometrySize = AllottedGeometry.GetLocalSize().GetMin() / 2.0f;
    return NormalSpace * GeometrySize * FVector2D(1.0f, -1.0f) + Center; // Need to reverse the Y axis
}

void SPokemonStatGraph::PaintLines(const FGeometry &AllottedGeometry, FSlateWindowElementList &OutDrawElements,
                                   int32 &DrawLayerId) const {
    if (CachedNodeLocations.Num() < 3) {
        return;
    }

    // Paint the radial lines
    auto CenterPoint = AllottedGeometry.GetLocalSize() / 2.0f;
    for (auto &MidPoint : CachedNodeLocations) {
        TArray<FVector2D> LinePoints;
        LinePoints.Reserve(2);
        LinePoints.Add(CenterPoint);
        LinePoints.Add(MidPoint);
        FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints,
                                     ESlateDrawEffect::None, GridLinesColor, true);
    }

    // Draw the lines that connect each node, adding an extra one for closing the loop
    TArray<FVector2D> LinePoints = CachedNodeLocations;
    LinePoints.Add(CachedNodeLocations[0]);
    FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints,
                                 ESlateDrawEffect::None, NodeLinesColor, true);
    DrawLayerId++;
}
