// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbook/SimpleFlipbook.h"
#include "PaperSprite.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Algorithm/Reduce.h"
#include "Ranges/Casting/StaticCast.h"
#include "Ranges/Optional/CastType.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Utilities/Arithmetic.h"
#include "Ranges/Utilities/GetTupleValue.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Enumerate.h"
#include "Ranges/Views/FilterTuple.h"
#include "Ranges/Views/Map.h"

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(
        TEXT("/Paper2D/MaskedUnlitSpriteMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}

int32 USimpleFlipbook::GetNumFrames() const {
    // clang-format off
    return KeyFrames |
           UE::Ranges::Map(&FSimpleFlipbookKeyFrame::FrameRun) |
           UE::Ranges::Reduce(0, UE::Ranges::Add);
    // clang-format on
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return GetNumFrames() / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds) {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f) {
        // clang-format off
        return KeyFrames |
               UE::Ranges::Map(&FSimpleFlipbookKeyFrame::FrameRun) |
               UE::Ranges::Map(UE::Ranges::Divide, FramesPerSecond) |
               UE::Ranges::Enumerate |
               UE::Ranges::FilterTuple([Time](size_t, float SumTime) { return Time <= SumTime; }) |
               UE::Ranges::Map(UE::Ranges::GetFromTuple<0>) |
               UE::Ranges::FindFirst |
               UE::Optionals::Map(UE::Ranges::StaticCast<int32>) |
               UE::Optionals::OrElse(KeyFrames.Num() - 1);
        // clang-format on
    }

    return KeyFrames.Num() > 0 ? 0 : INDEX_NONE;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    // clang-format off
    return UE::Optionals::OfNullable(ReferenceSprite) |
           UE::Optionals::Map(&UPaperSprite::GetRenderBounds) |
           UE::Optionals::OrElse(FBoxSphereBounds(ForceInit));
    // clang-format on
}

void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }

    //@TODO: Determine when this is really needed, as it is seriously expensive!
    //TComponentReregisterContext<UPaperFlipbookComponent> ReregisterAnimatedComponents;

    Super::PostEditChangeProperty(PropertyChangedEvent);
}