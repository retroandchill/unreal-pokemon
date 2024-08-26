// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DBaseEditorViewportClient.h"
#include "CanvasTypes.h"
#include "ImageUtils.h"

FTilemap3DBaseEditorViewportClient::FTilemap3DBaseEditorViewportClient(const TWeakPtr<SEditorViewport> &InEditorViewportWidget)
    : FEditorViewportClient(nullptr, nullptr, InEditorViewportWidget) {
    ELevelViewportType NewViewportType = LVT_Perspective;
    SetViewModes(VMI_Unlit, VMI_Unlit);
    SetViewportType(NewViewportType);

    bDeferZoomToSprite = true;
    bDeferZoomToSpriteIsInstant = true;

    // Get the correct general direction of the perspective mode; the distance doesn't matter much as we've queued up a deferred zoom that will calculate a much better distance
    auto InitialLocation = FVector(0, -1, -1);
    auto InitialRotation = FRotator(-45, -90, 0);
    SetInitialViewTransform(LVT_Perspective, -100.0f * InitialLocation, InitialRotation, DEFAULT_ORTHOZOOM);
}

void FTilemap3DBaseEditorViewportClient::Tick(float DeltaSeconds) {
    if (auto Size = Viewport->GetSizeXY(); bDeferZoomToSprite && (Size.X > 0) && (Size.Y > 0)) {
        auto BoundsToFocus = GetDesiredFocusBounds();
        if (ViewportType != LVT_Perspective) {
            TGuardValue SaveViewportType(ViewportType, LVT_Perspective);
            FocusViewportOnBox(BoundsToFocus, bDeferZoomToSpriteIsInstant);
        }

        FocusViewportOnBox(BoundsToFocus, bDeferZoomToSpriteIsInstant);
        bDeferZoomToSprite = false;
    }

    FEditorViewportClient::Tick(DeltaSeconds);
}

FLinearColor FTilemap3DBaseEditorViewportClient::GetBackgroundColor() const {
    return FLinearColor(0, 0, 127, 0);
}

void FTilemap3DBaseEditorViewportClient::AddReferencedObjects(FReferenceCollector &Collector) {
    FEditorViewportClient::AddReferencedObjects(Collector);
    Collector.AddReferencedObject(CheckerboardTexture);
}

void FTilemap3DBaseEditorViewportClient::RequestFocusOnSelection(bool bInstant) {
    bDeferZoomToSprite = true;
    bDeferZoomToSpriteIsInstant = bInstant;
}

void FTilemap3DBaseEditorViewportClient::ModifyCheckerboardTextureColors() {
    constexpr auto ColorOne = FColor(128, 128, 128);
    constexpr auto ColorTwo = FColor(64, 64, 64);
    constexpr int32 CheckerSize = 16;

    DestroyCheckerboardTexture();
    SetupCheckerboardTexture(ColorOne, ColorTwo, CheckerSize);
}

void FTilemap3DBaseEditorViewportClient::SetupCheckerboardTexture(const FColor &ColorOne, const FColor &ColorTwo,
    int32 CheckerSize) {
    if (CheckerboardTexture == nullptr) {
        CheckerboardTexture = FImageUtils::CreateCheckerboardTexture(ColorOne, ColorTwo, CheckerSize);
    }
}

void FTilemap3DBaseEditorViewportClient::DestroyCheckerboardTexture() {
    if (CheckerboardTexture) {
        if (CheckerboardTexture->GetResource()) {
            CheckerboardTexture->ReleaseResource();
        }
        
        CheckerboardTexture->MarkAsGarbage();
        CheckerboardTexture = nullptr;
    }
}

UTexture2D *FTilemap3DBaseEditorViewportClient::GetCheckerboardTexture() const {
    return CheckerboardTexture;
}

void FTilemap3DBaseEditorViewportClient::SetCheckerboardTexture(UTexture2D* InCheckerboardTexture) {
    CheckerboardTexture = InCheckerboardTexture;
}

FVector2D FTilemap3DBaseEditorViewportClient::GetZoomPos() const {
    return ZoomPos;
}

void FTilemap3DBaseEditorViewportClient::SetZoomPos1(FVector2D InZoomPos) {
    ZoomPos = InZoomPos;
}

float FTilemap3DBaseEditorViewportClient::GetZoomAmount() const {
    return ZoomAmount;
}

void FTilemap3DBaseEditorViewportClient::SetZoomAmount(float InZoomAmount) {
    ZoomAmount = InZoomAmount;
}