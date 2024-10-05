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