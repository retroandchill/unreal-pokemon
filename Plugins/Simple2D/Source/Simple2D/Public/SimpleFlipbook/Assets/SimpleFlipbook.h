// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "PaperSpriteAtlas.h"
#include "UObject/Object.h"
#include "SpriteEditorOnlyTypes.h"
#include "SimpleFlipbook.generated.h"

namespace EFlipbookCollisionMode {
    enum Type : int;
}

class UPaperSprite;

USTRUCT()
struct FSimpleFlipbookKeyFrame {
    GENERATED_BODY()

    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=0))
    int32 Index = 0;

    UPROPERTY(Category=Sprite, EditAnywhere, meta=(ClampMin=1))
    int32 FrameRun = 1;
    
};

/**
 * 
 */
UCLASS()
class SIMPLE2D_API USimpleFlipbook : public UObject {
    GENERATED_BODY()

public:
    USimpleFlipbook();

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UTexture2D* GetSourceTexture() const {
        return SourceTexture;
    }
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UPaperSprite* GetReferenceSprite() const {
        return ReferenceSprite;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    TEnumAsByte<EFlipbookCollisionMode::Type> GetCollisionSource() const { return CollisionSource; }

    bool ContainsSprite(UPaperSprite* Sprite) const {
        return Sprite == ReferenceSprite;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetNumFrames() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetRows() const {
        return Rows;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetColumns() const {
        return Columns;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    float GetFramesPerSecond() const {
        return FramesPerSecond;
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumKeyFrames() const {
        return KeyFrames.Num();
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    float GetTotalDuration() const;

    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    const FSimpleFlipbookKeyFrame &GetKeyFrameChecked(int32 Index) const {
        return KeyFrames[Index];
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return KeyFrames.IsValidIndex(Index);
    }

    FBoxSphereBounds GetRenderBounds() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface* GetDefaultMaterial() const {
        return DefaultMaterial;
    }

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSourceTexture, Category = Sprite)
    TObjectPtr<UTexture2D> SourceTexture;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetReferenceSprite, Category = Sprite)
    TObjectPtr<UPaperSprite> ReferenceSprite;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetCollisionSource, Category=Sprite, meta = (InvalidEnumValues = EachFrameCollision))
    TEnumAsByte<EFlipbookCollisionMode::Type> CollisionSource;

    UPROPERTY(EditAnywhere, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    TArray<FSimpleFlipbookKeyFrame> KeyFrames;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetRows, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Rows = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetColumns, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Columns = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetFramesPerSecond, Category = Sprite, meta=(ClampMin=0, ClampMax=1000))
    float FramesPerSecond = 15.f;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultMaterial, Category = Sprite)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

};
