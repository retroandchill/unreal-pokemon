// "Unreal Pokémon" created by Retro & Chill.


#include "PaperZDFlipbookHelpers.h"
#include "AnimSequences/PaperZDAnimSequence_Flipbook.h"

UPaperFlipbook * UPaperZDFlipbookHelpers::GetPreviewFlipbook(UPaperZDAnimSequence *Sequence,
                                                             EFacingDirection Direction) {
    float RenderOrientation = 0.f;
    switch (Direction) {
    using enum EFacingDirection;
    case Down:
        RenderOrientation = 180.f;
        break;
    case Left:
        RenderOrientation = -90.f;
        break;
    case Right:
        RenderOrientation = 90.f;
        break;
    case Up:
        RenderOrientation = 0.f;
        break;
    }

    auto FlipbookSequence = Cast<UPaperZDAnimSequence_Flipbook>(Sequence);
    if (FlipbookSequence == nullptr) {
        return nullptr;
    }

    return FlipbookSequence->GetAnimationData<UPaperFlipbook*>(RenderOrientation);
}