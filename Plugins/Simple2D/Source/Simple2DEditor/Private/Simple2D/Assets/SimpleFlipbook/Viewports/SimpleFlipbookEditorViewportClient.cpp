// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Assets/SimpleFlipbook/Viewports/SimpleFlipbookEditorViewportClient.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Utils.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Components/SimpleFlipbookComponent.h"
#include "Simple2D/Settings/SimpleFlipbookEditorSettings.h"


namespace Simple2D {
    FSimpleFlipbookEditorViewportClient::FSimpleFlipbookEditorViewportClient(
        const TAttribute<USimpleFlipbook *> &InFlipbookBeingEdited) : FlipbookBeingEdited(InFlipbookBeingEdited), FlipbookBeingEditedLastFrame(FlipbookBeingEdited.Get()), AnimatedRenderComponent(NewObject<USimpleFlipbookComponent>()) {
        PreviewScene = &OwnedPreviewScene;

        SetRealtime(true);
        
        AnimatedRenderComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
        AnimatedRenderComponent->SetFlipbook(FlipbookBeingEdited.Get());
        AnimatedRenderComponent->UpdateBounds();
        PreviewScene->AddComponent(AnimatedRenderComponent.Get(), FTransform::Identity);

        EngineShowFlags.DisableAdvancedFeatures();
        EngineShowFlags.SetCompositeEditorPrimitives(true);
    }

    void FSimpleFlipbookEditorViewportClient::Draw(const FSceneView *View, FPrimitiveDrawInterface *PDI) {
        FEditorViewportClient::Draw(View, PDI);

        if (bShowPivot && AnimatedRenderComponent.IsValid()) {
            FUnrealEdUtils::DrawWidget(View, PDI, AnimatedRenderComponent->GetComponentTransform().ToMatrixWithScale(), 0, 0, EAxisList::Screen, EWidgetMovementMode::WMM_Translate);
        }
    }

    void FSimpleFlipbookEditorViewportClient::DrawCanvas(FViewport &InViewport, FSceneView &View, FCanvas &Canvas) {
        FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);

        if (const bool bIsHitTesting = Canvas.IsHitTesting(); !bIsHitTesting) {
            Canvas.SetHitProxy(nullptr);
        }

        static const FText FlipbookHelpStr = NSLOCTEXT("SimpleFlipbook", "SimpleFlipbookEditHelp", "Flipbook editor\n\nAdd keys using the toolbar or by drag-dropping sprite assets\nChange the timeline scale using Ctrl+MouseWheel\nEdit keys using the handles and right-click menu\nReorder keys by dragging and dropping");

        int32 YPos = 42;
        // Display tool help
        FCanvasTextItem TextItem(FVector2D(6, YPos), FlipbookHelpStr, GEngine->GetSmallFont(), FLinearColor::White);
        TextItem.EnableShadow(FLinearColor::Black);
        TextItem.Draw(&Canvas);
    }

    void FSimpleFlipbookEditorViewportClient::Tick(float DeltaSeconds) {
        if (AnimatedRenderComponent.IsValid())
        {
            if (auto* Flipbook = FlipbookBeingEdited.Get(); Flipbook != FlipbookBeingEditedLastFrame.Get())
            {
                AnimatedRenderComponent->SetFlipbook(Flipbook);
                AnimatedRenderComponent->UpdateBounds();
                FlipbookBeingEditedLastFrame = Flipbook;
            }
        }
        
        FSimple2DEditorViewportClient::Tick(DeltaSeconds);

	    OwnedPreviewScene.GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
    }

    bool FSimpleFlipbookEditorViewportClient::InputKey(const FInputKeyEventArgs &EventArgs) {
        bool bHandled = false;

        // Pass keys to standard controls, if we didn't consume input
        return bHandled ? true : FEditorViewportClient::InputKey(EventArgs);
    }

    FLinearColor FSimpleFlipbookEditorViewportClient::GetBackgroundColor() const {
        return GetDefault<USimpleFlipbookEditorSettings>()->BackgroundColor;
    }

    FBox FSimpleFlipbookEditorViewportClient::GetDesiredFocusBounds() const {
        return AnimatedRenderComponent->Bounds.GetBox();
    }
}