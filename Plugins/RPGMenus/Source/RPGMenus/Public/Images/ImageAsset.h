// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Utilities/VariantObject.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "UObject/Object.h"
#include "ImageAsset.generated.h"

#if CPP
using FImageAsset = UE::Ranges::TVariantObject<UTexture, UMaterialInterface, ISlateTextureAtlasInterface>;
#else
USTRUCT(BlueprintType, NoExport)
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

static_assert(UE::Ranges::UEStruct<FImageAsset>);