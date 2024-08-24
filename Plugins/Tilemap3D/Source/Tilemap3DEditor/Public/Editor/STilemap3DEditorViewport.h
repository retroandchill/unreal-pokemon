// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class ATilemap3D;

/**
 * Editor viewport used for handling the editing of a 3D tilemap in the world.
 */
class TILEMAP3DEDITOR_API STilemap3DEditorViewport : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(STilemap3DEditorViewport) {
        }

    SLATE_END_ARGS()

    /**
     * Constructs this widget with the provided arguments
     * @param InArgs The arguments provided to slate
     */
    void Construct(const FArguments &InArgs);

    void SetTilemap(ATilemap3D* Tilemap3D);

    int32 OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

    FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
    
private:
    TWeakObjectPtr<ATilemap3D> Tilemap;
};
