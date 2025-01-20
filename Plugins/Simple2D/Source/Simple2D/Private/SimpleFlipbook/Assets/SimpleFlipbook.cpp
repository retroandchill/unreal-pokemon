// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbook/Assets/SimpleFlipbook.h"
#include "PaperSprite.h"

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(TEXT("/Paper2D/MaskedUnlitSpriteMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return static_cast<float>(GetNumFrames()) / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds) {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f) {
        return FMath::Min(FMath::FloorToInt32(Time / FramesPerSecond), GetNumKeyFrames() - 1);
    }
    
    return GetNumKeyFrames() > 0 ? 0 : INDEX_NONE;
}

FSimpleFlipbookKeyFrame USimpleFlipbook::GetKeyFrameChecked(int32 Index) const {
    check(IsValidKeyFrameIndex(Index))
    return { .Row = Index / Columns, .Column = Index % Columns };
}

bool USimpleFlipbook::IsValidKeyFrameIndex(int32 Index) const {
    return Index < 0 || Index >= GetNumKeyFrames();
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