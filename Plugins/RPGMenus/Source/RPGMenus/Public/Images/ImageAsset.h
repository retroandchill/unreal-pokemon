// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Variants/VariantObject.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "UObject/Object.h"

#include "ImageAsset.generated.h"

#if CPP
UE_DECLARE_VARIANT_OBJECT_STRUCT(FImageAsset, UTexture, UMaterialInterface, ISlateTextureAtlasInterface);
#else
USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FImageAsset {
    UPROPERTY(EditAnywhere,
        meta = (AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
            DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};
#endif

template<>
struct RPGMENUS_API TBaseStructure<FImageAsset>  {
    static UScriptStruct* Get(); 
};

/**
 * Blueprint helper functions for a variant type.
 */
UCLASS()
class RPGMENUS_API UImageAssetHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Variants, meta = (ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult MakeImageAsset(UObject* Object, FImageAsset& AsImageAsset);
    
    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Texture)", Category = Variants, meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_Texture(UTexture* Texture);
    
    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Material)", Category = Variants, meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_Material(UMaterialInterface* Material);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Slate Texture Atlas)", Category = Variants, meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_SlateTextureAsset(const TScriptInterface<ISlateTextureAtlasInterface>& TextureAtlas);

    UFUNCTION(BlueprintPure, Category = Variants,
              meta = (CompactNodeTitle = "->", BlueprintAutocast, AutoCreateRefTerm = ImageAsset))
    static UObject* ConvertToObject(const FImageAsset& ImageAsset);

    UFUNCTION(BlueprintCallable, Category = Variants,
        meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToTexture(const FImageAsset& ImageAsset, UTexture*& AsTexture);

    UFUNCTION(BlueprintCallable, Category = Variants,
        meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToMaterial(const FImageAsset& ImageAsset, UMaterialInterface*& AsMaterial);

    UFUNCTION(BlueprintCallable, Category = Variants,
        meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToSlateTextureAtlas(const FImageAsset& ImageAsset, TScriptInterface<ISlateTextureAtlasInterface>& AsTextureAtlas);
};