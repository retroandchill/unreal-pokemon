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
    USimpleFlipbook();
    
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    // Returns the collision source of this flipbook animation (if any)
    TEnumAsByte<EFlipbookCollisionMode::Type> GetCollisionSource() const { return CollisionSource; }

    // Returns the nominal frame rate to play this flipbook animation back at
    float GetFramesPerSecond() const;

    // Returns the total duration in seconds
    UFUNCTION(BlueprintCallable, Category="Sprite")
    float GetTotalDuration() const;

    // Returns the keyframe index that covers the specified time (in seconds), or INDEX_NONE if none exists.
    // When bClampToEnds is true, it will choose the first or last keyframe if the time is out of range.
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    // Returns the render bounds of this sprite
    FBoxSphereBounds GetRenderBounds() const;

    // Returns the number of key frames
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumKeyFrames() const {
        return TotalFrames;
    }

    // Is the specified Index within the valid range of key frames?
    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return Index >= 0 && Index < TotalFrames;
    }

    // Return the default material for this flipbook
    UMaterialInterface *GetDefaultWorldMaterial() const {
        return DefaultWorldMaterial;
    }

    UMaterialInterface *GetDefaultWidgetMaterial() const {
        return DefaultWidgetMaterial;
    }

private:
    UPROPERTY(Category = Sprite, EditAnywhere)
    TObjectPtr<UTexture2D> SourceTexture;
    
    // The nominal frame rate to play this flipbook animation back at
    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=0, ClampMax=1000))
    float FramesPerSecond = 15.f;
    
    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=0, ClampMax=1000))
    int32 TotalFrames = 1;

    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=0, ClampMax=1000))
    int32 Columns = 1;

    // The set of key frames for this flipbook animation (each one has a duration and a sprite to display)
    UPROPERTY(Category=Sprite, EditAnywhere)
    TObjectPtr<UPaperSprite> ReferenceSprite;

    // The material to use on a flipbook player instance if not overridden
    UPROPERTY(Category=Sprite, EditAnywhere)
    TObjectPtr<UMaterialInterface> DefaultWorldMaterial;

    UPROPERTY(Category=Sprite, EditAnywhere)
    TObjectPtr<UMaterialInterface> DefaultWidgetMaterial;

    // Collision source
    UPROPERTY(Category=Sprite, EditAnywhere, BlueprintReadOnly,
        meta = (ValidEnumValues="NoCollision,FirstFrameCollision"))
    TEnumAsByte<EFlipbookCollisionMode::Type> CollisionSource;

};