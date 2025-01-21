// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "PaperSpriteAtlas.h"
#include "UObject/Object.h"
#include "SpriteEditorOnlyTypes.h"
#include "SimpleFlipbook.generated.h"

class UPaperSprite;

USTRUCT()
struct FSimpleFlipbookKeyFrame {
    GENERATED_BODY()

    UPROPERTY()
    int32 Row = 0;

    UPROPERTY()
    int32 Column = 0;
    
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
    UPaperSprite* GetReferenceSprite() const {
        return ReferenceSprite;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetNumFrames() const {
        return TotalFrames;
    }

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
        return GetNumFrames();
    }

    UFUNCTION(BlueprintCallable, Category="Sprite")
    float GetTotalDuration() const;

    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    const FSimpleFlipbookKeyFrame& GetKeyFrameChecked(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const;

    FBoxSphereBounds GetRenderBounds() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface* GetDefaultMaterial() const {
        return DefaultMaterial;
    }

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetReferenceSprite, Category = Sprite)
    TObjectPtr<UPaperSprite> ReferenceSprite;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetNumFrames, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 TotalFrames = 1;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetRows, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Rows = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetColumns, Category = Sprite, meta = (ClamMin = 1, UIMin = 1))
    int32 Columns = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetFramesPerSecond, Category = Sprite, meta=(ClampMin=0, ClampMax=1000))
    float FramesPerSecond = 15.f;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultMaterial, Category = Sprite)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

};
