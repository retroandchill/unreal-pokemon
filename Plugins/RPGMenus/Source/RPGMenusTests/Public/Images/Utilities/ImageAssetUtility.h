// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Images/ImageAsset.h"
#include "UObject/Interface.h"
#include "ImageAssetUtility.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPGMENUSTESTS_API UImageAssetUtility : public UObject {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    TArray<FImageAsset> CreateImageAssets(UTexture2D* Texture2D, const TScriptInterface<ISlateTextureAtlasInterface>& Sprite) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool TryCreateImageAsset(const UObject* Object, FImageAsset& CreatedAsset) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool GetTextureFromImageAsset(const FImageAsset& Asset, UTexture2D*& OutTexture) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool GetSpriteFromImageAsset(const FImageAsset& Asset, TScriptInterface<ISlateTextureAtlasInterface>& OutSprite) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    UObject* GetObjectFromImageAsset(const FImageAsset& CreatedAsset) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    FSoftImageAsset MakeSoftImageAsset(const FImageAsset& CreatedAsset) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    FSoftImageAsset MakeSoftImageAssetFromMaterial(const TSoftObjectPtr<UMaterialInterface>& Material) const;
    
    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool TryMakeSoftImageAsset(const TSoftObjectPtr<UObject>& Object, FSoftImageAsset& OutSoftAsset) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool GetSoftMaterialFromImageAsset(const FSoftImageAsset& Asset, TSoftObjectPtr<UMaterialInterface>& OutTexture) const;

    UFUNCTION(BlueprintImplementableEvent, Category = UnitTesting)
    bool LoadSynchronous(const FSoftImageAsset& SoftImageAsset, FImageAsset& OutAsset) const;

    UPROPERTY(EditDefaultsOnly, Category = UnitTesting)
    TObjectPtr<UTexture2D> TextureAsset;

    UPROPERTY(EditDefaultsOnly, Category = UnitTesting)
    TObjectPtr<UPaperSprite> SpriteAsset;

    UPROPERTY(EditDefaultsOnly, Category = UnitTesting)
    TSoftObjectPtr<UMaterialInterface> MaterialAsset;

    UPROPERTY(EditDefaultsOnly, Category = UnitTesting)
    TObjectPtr<USoundBase> InvalidAsset;

    UPROPERTY(EditDefaultsOnly, Category = UnitTesting)
    TSoftObjectPtr<USoundBase> InvalidSoftAsset;
};