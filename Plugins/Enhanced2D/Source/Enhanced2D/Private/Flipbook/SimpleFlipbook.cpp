// "Unreal Pokémon" created by Retro & Chill.


#include "Flipbook/SimpleFlipbook.h"
#include "PaperSprite.h"

#if WITH_EDITOR
void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

bool USimpleFlipbook::ContainsSprite(UPaperSprite *SpriteAsset) const {
    for (const FPaperFlipbookKeyFrame &KeyFrame : KeyFrames) {
        if (KeyFrame.Sprite == SpriteAsset) {
            return true;
        }
    }

    return false;
}

float USimpleFlipbook::GetFramesPerSecond() const {
    return FramesPerSecond;
}

int32 USimpleFlipbook::GetNumFrames() const {
    int32 SumFrames = 0;
    for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex) {
        SumFrames += KeyFrames[KeyFrameIndex].FrameRun;
    }

    return SumFrames;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return GetNumFrames() / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if ((Time < 0.0f) && !bClampToEnds) {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f) {
        float SumTime = 0.0f;

        for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex) {
            SumTime += KeyFrames[KeyFrameIndex].FrameRun / FramesPerSecond;

            if (Time <= SumTime) {
                return KeyFrameIndex;
            }
        }

        // Return the last frame (note: relies on INDEX_NONE = -1 if there are no key frames)
        return KeyFrames.Num() - 1;
    }
    
    return (KeyFrames.Num() > 0) ? 0 : INDEX_NONE;
}

UPaperSprite *USimpleFlipbook::GetSpriteAtTime(float Time, bool bClampToEnds) const {
    const int32 KeyFrameIndex = GetKeyFrameIndexAtTime(Time, bClampToEnds);
    return KeyFrames.IsValidIndex(KeyFrameIndex) ? KeyFrames[KeyFrameIndex].Sprite : nullptr;
}

UPaperSprite *USimpleFlipbook::GetSpriteAtFrame(int32 FrameIndex) const {
    return KeyFrames.IsValidIndex(FrameIndex) ? KeyFrames[FrameIndex].Sprite : nullptr;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    FBoxSphereBounds MergedBoundingBox(ForceInit);
    bool bHasValidBounds = false;

    for (const FPaperFlipbookKeyFrame &KeyFrame : KeyFrames) {
        if (KeyFrame.Sprite != nullptr) {
            const FBoxSphereBounds FrameBounds = KeyFrame.Sprite->GetRenderBounds();

            if (bHasValidBounds) {
                MergedBoundingBox = Union(MergedBoundingBox, FrameBounds);
            } else {
                MergedBoundingBox = FrameBounds;
                bHasValidBounds = true;
            }
        }
    }

    return MergedBoundingBox;
}

bool USimpleFlipbook::FindSocket(FName SocketName, int32 KeyFrameIndex, FTransform &OutLocalTransform) {
    if (KeyFrames.IsValidIndex(KeyFrameIndex) && (SocketName != NAME_None)) {
        if (UPaperSprite *SpriteFrame = KeyFrames[KeyFrameIndex].Sprite) {
            if (FPaperSpriteSocket *SocketInfo = SpriteFrame->FindSocket(SocketName)) {
                OutLocalTransform = SocketInfo->LocalTransform;
                OutLocalTransform.ScaleTranslation(SpriteFrame->GetUnrealUnitsPerPixel());

                return true;
            }
        }
    }

    return false;
}

bool USimpleFlipbook::HasAnySockets() const {
    for (const FPaperFlipbookKeyFrame &KeyFrame : KeyFrames) {
        if (KeyFrame.Sprite != nullptr && KeyFrame.Sprite->HasAnySockets()) {
            return true;
        }
    }

    return false;
}

bool USimpleFlipbook::DoesSocketExist(FName SocketName) const {
    for (const FPaperFlipbookKeyFrame &KeyFrame : KeyFrames) {
        if (KeyFrame.Sprite != nullptr && KeyFrame.Sprite->FindSocket(SocketName)) {
            return true;
        }
    }

    return false;
}

void USimpleFlipbook::QuerySupportedSockets(TArray<FComponentSocketDescription> &OutSockets) const {
    TSet<FName> SocketNames;
    TArray<FComponentSocketDescription> FrameDescriptions;

    for (const FPaperFlipbookKeyFrame &KeyFrame : KeyFrames) {
        if (KeyFrame.Sprite == nullptr) {
            continue;
        }
        
        FrameDescriptions.Reset();
        KeyFrame.Sprite->QuerySupportedSockets(/*out*/ FrameDescriptions);

        for (const FComponentSocketDescription &FrameSocket : FrameDescriptions) {
            if (SocketNames.Contains(FrameSocket.Name)) {
                continue;
            }
            
            SocketNames.Add(FrameSocket.Name);
            OutSockets.Add(FrameSocket);
        }
    }
}