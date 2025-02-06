// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "RetroLib/Variants/SoftVariantObject.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"

#if WITH_PAPERZD
#include "AnimSequences/PaperZDAnimSequence.h"
#endif

#include "BattleRender.generated.h"

#define BATTLE_RENDER_BASE_TYPES USkeletalMesh, UStaticMesh, UPaperSprite, UPaperFlipbook, USimpleFlipbook
#if WITH_PAPERZD
#define BATTLE_RENDER_TYPES BATTLE_RENDER_BASE_TYPES, UPaperZDAnimSequence
#else
#define BATTLE_RENDER_TYPES BATTLE_RENDER_BASE_TYPES
#endif

struct FSoftBattleRender;

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FBattleRender
#if CPP
    : Retro::TVariantObject<BATTLE_RENDER_TYPES> {
#else
{
#endif
    RETRO_VARIANT_OBJECT_STRUCT_BODY(FBattleRender, FSoftBattleRender)

#if !CPP
  private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
#endif
};

RETRO_DECLARE_VARIANT_OBJECT_STRUCT(FBattleRender);

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftBattleRender
#if CPP
    : Retro::TSoftVariantObject<FBattleRender> {
#else
{
#endif
    RETRO_SOFT_VARIANT_OBJECT_STRUCT_BODY(FSoftBattleRender)

#if !CPP
  private:
    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
#endif
};

RETRO_DECLARE_SOFT_VARIANT_OBJECT_STRUCT(FSoftBattleRender);

template <>
struct POKEMONASSETS_API TBaseStructure<FBattleRender> {
    static UScriptStruct *Get();
};

template <>
struct POKEMONASSETS_API TBaseStructure<FSoftBattleRender> {
    static UScriptStruct *Get();
};