// "Unreal Pokémon" created by Retro & Chill.


#include "Flipbook/SimpleFlipbook.h"
#include "PaperSprite.h"

USimpleFlipbook::USimpleFlipbook() {
    using FMaterialFinder = ConstructorHelpers::FObjectFinder<UMaterialInterface>;
    
    static FMaterialFinder WorldMaterial(TEXT("/Enhanced2D/Materials/MaskedUnlitFlipbookMaterial"));
    DefaultWorldMaterial = WorldMaterial.Object;

    static FMaterialFinder WidgetMaterial(TEXT("/Enhanced2D/Materials/MaskedWidgetFlipbookMaterial"));
    DefaultWidgetMaterial = WidgetMaterial.Object;
}

#if WITH_EDITOR
void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

float USimpleFlipbook::GetFramesPerSecond() const {
    return FramesPerSecond;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return GetNumKeyFrames() / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds) {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f) {
        float SumTime = 0.0f;

        for (int32 KeyFrameIndex = 0; KeyFrameIndex < TotalFrames; ++KeyFrameIndex) {
            SumTime += 1.f / FramesPerSecond;

            if (Time <= SumTime) {
                return KeyFrameIndex;
            }
        }

        // Return the last frame (note: relies on INDEX_NONE = -1 if there are no key frames)
        return TotalFrames - 1;
    }
    
    return TotalFrames > 0 ? 0 : INDEX_NONE;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    if (ReferenceSprite == nullptr) {
        return FBoxSphereBounds(ForceInit);
    }
    
    return ReferenceSprite->GetRenderBounds();
}