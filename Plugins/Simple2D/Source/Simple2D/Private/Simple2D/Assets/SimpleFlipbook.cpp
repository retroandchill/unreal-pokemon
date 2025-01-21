// "Unreal Pokémon" created by Retro & Chill.


#include "../../../Public/Simple2D"
#include "Paper2DModule.h"
#include "PaperSprite.h"

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(TEXT("/Simple2D/Materials/MaskedUnlitFlipbookMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}

int32 USimpleFlipbook::GetNumFrames() const {
    int32 SumFrames = 0;
    for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex)
    {
        SumFrames += KeyFrames[KeyFrameIndex].FrameRun;
    }

    return SumFrames;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return static_cast<float>(GetNumFrames()) / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds)
    {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f)
    {
        float SumTime = 0.0f;

        for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex)
        {
            SumTime += KeyFrames[KeyFrameIndex].FrameRun / FramesPerSecond;

            if (Time <= SumTime)
            {
                return KeyFrameIndex;
            }
        }

        // Return the last frame (note: relies on INDEX_NONE = -1 if there are no key frames)
        return KeyFrames.Num() - 1;
    }
    
    return KeyFrames.Num() > 0 ? 0 : INDEX_NONE;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    return IsValid(ReferenceSprite) ? ReferenceSprite->GetRenderBounds() : FBoxSphereBounds(FBox(ForceInit));
}

#if WITH_EDITOR
void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }
    
    UObject::PostEditChangeProperty(PropertyChangedEvent);
}
#endif