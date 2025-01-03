// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2DDynamic.h"
#include "PaperFlipbook.h"
#include "RetroLib/Variants/SoftVariantObject.h"
#include "Slate/SlateTextureAtlasInterface.h"

#include "ImageAsset.generated.h"

#if CPP
RETRO_DECLARE_VARIANT_OBJECT_STRUCT(ImageAsset, UTexture2D, UTexture2DDynamic, UMaterialInterface,
                                    ISlateTextureAtlasInterface, UPaperFlipbook);
#else
USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FImageAsset {
    // clang-format off
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.Texture2D,/Script/Engine.Texture2DDynamic,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface,/Script/Paper2D.PaperFlipbook",
                      DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    // clang-format on
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftImageAsset {
    // clang-format off
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.Texture2D,/Script/Engine.Texture2DDynamic,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface,/Script/Paper2D.PaperFlipbook",
                      DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    // clang-format on
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