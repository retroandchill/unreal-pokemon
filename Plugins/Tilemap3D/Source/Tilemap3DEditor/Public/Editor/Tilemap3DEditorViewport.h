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
    void SetMesh(UStaticMesh* InMesh);

    int32 OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;

    FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

    void OnMouseLeave(const FPointerEvent &MouseEvent) override;
    FReply OnMouseMove(const FGeometry& SenderGeometry, const FPointerEvent& MouseEvent) override;
    FReply OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
    FReply OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
    
private:
    TOptional<FIntVector2> GetGridPosition(const FVector2D& Geometry, double Scale = 1.0) const;
    void SetTile(const FIntVector2& Position, int32 Layer = 0) const;

    bool bIsHoldingMouse = false;
    TWeakObjectPtr<ATilemap3D> Tilemap;
    TWeakObjectPtr<UStaticMesh> Mesh;
    TOptional<FIntVector2> CurrentMousePosition;
};
