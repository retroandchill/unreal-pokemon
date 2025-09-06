// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInterface.h"
#include "PaperFlipbook.h"
#include "RetroLib/Variants/SoftVariantObject.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Slate/SlateTextureAtlasInterface.h"

#include "ImageAsset.generated.h"

struct FSoftImageAsset;

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FImageAsset
#if CPP
    : Retro::TVariantObject<UTexture2D, UTexture2DDynamic, UMaterialInterface, ISlateTextureAtlasInterface,
                            UPaperFlipbook, USimpleFlipbook>
{
#else
{
#endif
    RETRO_VARIANT_OBJECT_STRUCT_BODY(FImageAsset, FSoftImageAsset)

#if !CPP
  private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
#endif
};

RETRO_DECLARE_VARIANT_OBJECT_STRUCT(FImageAsset);

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftImageAsset
#if CPP
    : Retro::TSoftVariantObject<FImageAsset>
{
#else
{
#endif
    RETRO_SOFT_VARIANT_OBJECT_STRUCT_BODY(FSoftImageAsset)

#if !CPP
  private:
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
#endif
};

RETRO_DECLARE_SOFT_VARIANT_OBJECT_STRUCT(FSoftImageAsset);

template <>
struct RPGMENUS_API TBaseStructure<FImageAsset>
{
    static UScriptStruct *Get();
};

template <>
struct RPGMENUS_API TBaseStructure<FSoftImageAsset>
{
    static UScriptStruct *Get();
};