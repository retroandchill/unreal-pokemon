// "Unreal Pokémon" created by Retro & Chill.

#pragma once

namespace Simple2D
{
    struct FViewportSelectionRectangle
    {
        FVector2D TopLeft;
        FVector2D Dimensions;
        FLinearColor Color;
    };

    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimple2DEditorViewportClient : public FEditorViewportClient
    {
      public:
        explicit FSimple2DEditorViewportClient(const TWeakPtr<SEditorViewport> &InEditorViewportWidget = nullptr);

        void Tick(float DeltaSeconds) override;
        void AddReferencedObjects(FReferenceCollector &Collector) override;

        void RequestFocusOnSelection(bool bInstant);

        void ModifyCheckerboardTextureColors();

        void SetZoomPos(FVector2D InNewPos, float InNewZoom)
        {
            ZoomPos = InNewPos;
            ZoomAmount = InNewZoom;
        }

      private:
        void SetupCheckerboardTexture(const FColor &ColorOne, const FColor &ColorTwo, int32 CheckerSize);
        void DestroyCheckerboardTexture();

      protected:
        void DrawSelectionRectangles(FViewport *Viewport, FCanvas *Canvas);

        virtual FBox GetDesiredFocusBounds() const
        {
            return FBox(ForceInitToZero);
        }

      private:
        TArray<FViewportSelectionRectangle> SelectionRectangles;

        TObjectPtr<UTexture2D> CheckerboardTexture;
        FVector2D ZoomPos = FVector2D::ZeroVector;
        float ZoomAmount = 1.f;

        bool bDeferZoomToSprite = true;
        bool bDeferZoomToSpriteIsInstant = true;
    };
} // namespace Simple2D
