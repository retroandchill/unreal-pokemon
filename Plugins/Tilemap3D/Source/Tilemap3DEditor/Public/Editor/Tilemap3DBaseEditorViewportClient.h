// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace Tilemap3D {
    struct FViewportSelectionRectangle {
        FVector2D TopLeft;
        FVector2D Dimensions;
        FLinearColor Color;
    };
} // namespace Tilemap3D

/**
 *
 */
class TILEMAP3DEDITOR_API FTilemap3DBaseEditorViewportClient : public FEditorViewportClient {
  public:
    /** Constructor */
    explicit FTilemap3DBaseEditorViewportClient(const TWeakPtr<SEditorViewport> &InEditorViewportWidget = nullptr);

    void Tick(float DeltaSeconds) override;
    FLinearColor GetBackgroundColor() const override;
    void AddReferencedObjects(FReferenceCollector &Collector) override;

    void RequestFocusOnSelection(bool bInstant);

    /** Modifies the checkerboard texture's data */
    void ModifyCheckerboardTextureColors();

    void SetZoomPos(FVector2D InNewPos, float InNewZoom) {
        ZoomPos = InNewPos;
        ZoomAmount = InNewZoom;
    }

    // List of selection rectangles to draw
    TArray<Tilemap3D::FViewportSelectionRectangle> SelectionRectangles;

  private:
    /** Initialize the checkerboard texture for the texture preview, if necessary */
    void SetupCheckerboardTexture(const FColor &ColorOne, const FColor &ColorTwo, int32 CheckerSize);

    /** Destroy the checkerboard texture if one exists */
    void DestroyCheckerboardTexture();

  protected:
    virtual FBox GetDesiredFocusBounds() const {
        return FBox(ForceInitToZero);
    }

  private:
    /** Checkerboard texture */
    TObjectPtr<UTexture2D> CheckerboardTexture;
    FVector2D ZoomPos = FVector2D::ZeroVector;
    float ZoomAmount = 1.0f;

    // Should we zoom to the focus bounds next tick?
    bool bDeferZoomToSprite;
    bool bDeferZoomToSpriteIsInstant;
};
