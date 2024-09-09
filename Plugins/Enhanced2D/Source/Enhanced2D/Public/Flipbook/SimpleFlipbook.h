// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "UObject/Object.h"
#include "SimpleFlipbook.generated.h"

/**
 * Lightweight version of Paper Flipbook that doesn't require individual sprite objects for each frame.
 */
UCLASS()
class ENHANCED2D_API USimpleFlipbook : public UObject {
    GENERATED_BODY()

public:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    // Returns true if the flipbook asset contains the specified sprite asset in any frames
    bool ContainsSprite(UPaperSprite *SpriteAsset) const;

    // Returns the collision source of this flipbook animation (if any)
    TEnumAsByte<EFlipbookCollisionMode::Type> GetCollisionSource() const {
        return CollisionSource;
    }

    // Returns the nominal frame rate to play this flipbook animation back at
    float GetFramesPerSecond() const;

    // Returns the total number of frames
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumFrames() const;

    // Returns the total duration in seconds
    UFUNCTION(BlueprintCallable, Category="Sprite")
    float GetTotalDuration() const;

    // Returns the keyframe index that covers the specified time (in seconds), or INDEX_NONE if none exists.
    // When bClampToEnds is true, it will choose the first or last keyframe if the time is out of range.
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    // Returns the sprite at the specified time (in seconds), or nullptr if none exists.
    // When bClampToEnds is true, it will choose the first or last sprite if the time is out of range.
    UFUNCTION(BlueprintCallable, Category="Sprite")
    UPaperSprite *GetSpriteAtTime(float Time, bool bClampToEnds = false) const;

    // Returns the sprite at the specified keyframe index, or nullptr if none exists
    UFUNCTION(BlueprintCallable, Category="Sprite")
    UPaperSprite *GetSpriteAtFrame(int32 FrameIndex) const;

    // Returns the render bounds of this sprite
    FBoxSphereBounds GetRenderBounds() const;

    // Returns the number of key frames
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumKeyFrames() const {
        return KeyFrames.Num();
    }

    // Is the specified Index within the valid range of key frames?
    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return KeyFrames.IsValidIndex(Index);
    }

    // Returns the key frame at the specified index, make sure the index is valid before use
    const FPaperFlipbookKeyFrame &GetKeyFrameChecked(int32 Index) const {
        return KeyFrames[Index];
    }

    // Search for a socket at the specified frame
    bool FindSocket(FName SocketName, int32 KeyFrameIndex, FTransform &OutLocalTransform);

    // Returns true if the flipbook has any sockets
    bool HasAnySockets() const;

    // Returns true if the flipbook has a specific named socket
    bool DoesSocketExist(FName SocketName) const;

    // Returns a list of all of the sockets
    void QuerySupportedSockets(TArray<FComponentSocketDescription> &OutSockets) const;

    // Return the default material for this flipbook
    UMaterialInterface *GetDefaultMaterial() const {
        return DefaultMaterial;
    }

private:
    // The nominal frame rate to play this flipbook animation back at
    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=1000))
    float FramesPerSecond;

    // The set of key frames for this flipbook animation (each one has a duration and a sprite to display)
    UPROPERTY(Category=Sprite, EditAnywhere)
    TArray<FPaperFlipbookKeyFrame> KeyFrames;

    // The material to use on a flipbook player instance if not overridden
    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

    // Collision source
    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly)
    TEnumAsByte<EFlipbookCollisionMode::Type> CollisionSource;

    friend class FScopedFlipbookMutator;

};