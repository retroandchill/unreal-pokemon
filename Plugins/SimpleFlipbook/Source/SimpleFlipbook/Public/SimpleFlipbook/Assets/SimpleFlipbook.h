// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleFlipbook.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFLIPBOOK_API USimpleFlipbook : public UObject {
    GENERATED_BODY()

public:
    USimpleFlipbook();
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UTexture2D* GetSourceTexture() const {
        return SourceTexture;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetTotalFrames() const {
        return Columns;
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

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface* GetDefaultMaterial() const {
        return DefaultMaterial;
    }

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSourceTexture, Category = Sprite)
    TObjectPtr<UTexture2D> SourceTexture;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetTotalFrames, Category = Sprite)
    int32 TotalFrames = 1;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetRows, Category = Sprite)
    int32 Rows = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetColumns, Category = Sprite)
    int32 Columns = 1;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetFramesPerSecond, Category = Sprite)
    float FramesPerSecond = 15.f;
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultMaterial, Category = Sprite)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

};
