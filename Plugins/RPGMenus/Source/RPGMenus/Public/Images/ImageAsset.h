// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2DDynamic.h"
#include "PaperFlipbook.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Slate/SlateTextureAtlasInterface.h"

#include "ImageAsset.generated.h"

#if CPP
UE_DECLARE_VARIANT_OBJECT_STRUCT(ImageAsset, UTexture2D, UTexture2DDynamic, UMaterialInterface,
                                 ISlateTextureAtlasInterface, UPaperFlipbook);
#else
USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FImageAsset {
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.Texture2D,/Script/Engine.Texture2DDynamic,/Script/Engine.MaterialInterface,/"
                          "Script/Engine.SlateTextureAtlasInterface,/Script/Paper2D.PaperFlipbook",
                      DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftImageAsset {
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.Texture2D,/Script/Engine.Texture2DDynamic,/Script/Engine.MaterialInterface,/"
                          "Script/Engine.SlateTextureAtlasInterface,/Script/Paper2D.PaperFlipbook",
                      DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
};
#endif

template <>
struct RPGMENUS_API TBaseStructure<FImageAsset> {
    static UScriptStruct *Get();
};

template <>
struct RPGMENUS_API TBaseStructure<FSoftImageAsset> {
    static UScriptStruct *Get();
};

/**
 * Blueprint helper functions for a variant type.
 */
UCLASS()
class RPGMENUS_API UImageAssetHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Variants, meta = (ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult MakeImageAsset(UObject *Object, FImageAsset &AsImageAsset);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Texture)", Category = Variants,
              meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_Texture2D(UTexture2D *Texture);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Texture Dynamic)", Category = Variants,
              meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_Texture2DDynamic(UTexture2DDynamic *Texture);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Material)", Category = Variants,
              meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_Material(UMaterialInterface *Material);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Slate Texture Atlas)", Category = Variants,
              meta = (BlueprintAutocast))
    static FImageAsset
    MakeImageAsset_SlateTextureAsset(const TScriptInterface<ISlateTextureAtlasInterface> &TextureAtlas);

    UFUNCTION(BlueprintPure, DisplayName = "Make Image Asset (Paper Flipbook)", Category = Variants,
              meta = (BlueprintAutocast))
    static FImageAsset MakeImageAsset_PaperFlipbook(UPaperFlipbook *Flipbook);

    UFUNCTION(BlueprintPure, Category = Variants,
              meta = (CompactNodeTitle = "->", BlueprintAutocast, AutoCreateRefTerm = ImageAsset))
    static UObject *ConvertToObject(const FImageAsset &ImageAsset);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToTexture2D(const FImageAsset &ImageAsset, UTexture2D *&AsTexture);

    UFUNCTION(BlueprintCallable, Category = Variants, DisplayName = "Cast to Texture 2D Dyanmic",
              meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToTextureDynamic(const FImageAsset &ImageAsset, UTexture2DDynamic *&AsTexture);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToMaterial(const FImageAsset &ImageAsset, UMaterialInterface *&AsMaterial);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToSlateTextureAtlas(const FImageAsset &ImageAsset,
                                                      TScriptInterface<ISlateTextureAtlasInterface> &AsTextureAtlas);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "ImageAsset", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToPaperFlipbook(const FImageAsset &ImageAsset, UPaperFlipbook *&AsFlipbook);

    UFUNCTION(BlueprintPure, Category = Variants,
              meta = (CompactNodeTile = "->", BlueprintAutocast, AutoCreateRefTerm = ImageAsset))
    static FSoftImageAsset MakeSoftImageAsset(const FImageAsset &ImageAsset);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = Path, ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult LoadSynchronous(const FSoftImageAsset &Path, FImageAsset &LoadedAsset);
};