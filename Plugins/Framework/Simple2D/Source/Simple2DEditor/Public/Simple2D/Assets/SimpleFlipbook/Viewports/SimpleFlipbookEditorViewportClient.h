// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "Simple2D/Simple2DEditorViewportClient.h"

class USimpleFlipbookComponent;
class USimpleFlipbook;

namespace Simple2D {
    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimpleFlipbookEditorViewportClient : public FSimple2DEditorViewportClient {
      public:
        explicit FSimpleFlipbookEditorViewportClient(const TAttribute<USimpleFlipbook *> &InFlipbookBeingEdited);

        void Draw(const FSceneView *View, FPrimitiveDrawInterface *PDI) override;
        void DrawCanvas(FViewport &InViewport, FSceneView &View, FCanvas &Canvas) override;
        void Tick(float DeltaSeconds) override;

        bool InputKey(const FInputKeyEventArgs &EventArgs) override;
        FLinearColor GetBackgroundColor() const override;

        void ToggleShowPivot() {
            bShowPivot = !bShowPivot;
            Invalidate();
        }
        bool IsShowPivotChecked() const {
            return bShowPivot;
        }

        USimpleFlipbookComponent *GetPreviewComponent() const {
            return AnimatedRenderComponent.Get();
        }

      protected:
        FBox GetDesiredFocusBounds() const override;

      private:
        FPreviewScene OwnedPreviewScene;
        TAttribute<USimpleFlipbook *> FlipbookBeingEdited;

        TWeakObjectPtr<USimpleFlipbook> FlipbookBeingEditedLastFrame;
        TWeakObjectPtr<USimpleFlipbookComponent> AnimatedRenderComponent;

        bool bShowPivot = true;
    };
} // namespace Simple2D