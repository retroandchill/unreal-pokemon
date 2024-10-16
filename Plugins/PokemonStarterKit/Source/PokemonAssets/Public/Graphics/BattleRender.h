// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "Ranges/Variants/VariantObjectStruct.h"

#if WITH_PAPERZD
#include "AnimSequences/PaperZDAnimSequence.h"
#endif

#include "BattleRender.generated.h"

#if CPP
#define BATTLE_RENDER_BASE_TYPES USkeletalMesh, UStaticMesh, UPaperSprite, UPaperFlipbook
#if WITH_PAPERZD
#define BATTLE_RENDER_TYPES BATTLE_RENDER_BASE_TYPES, UPaperZDAnimSequence
#else
#define BATTLE_RENDER_TYPES BATTLE_RENDER_BASE_TYPES
#endif

UE_DECLARE_VARIANT_OBJECT_STRUCT(BattleRender, BATTLE_RENDER_TYPES);
#else
USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FBattleRender {
    // clang-format off
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.SkeletalMesh,/Script/Engine.StaticMesh,/Script/Paper2D.PaperSprite,/Script/Paper2D.PaperFlipbook,/Script/PaperZD.PaperZDAnimSequence"))
    // clang-format on
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftBattleRender {
    // clang-format off
    UPROPERTY(EditAnywhere,
              meta = (AllowedClasses =
                          "/Script/Engine.SkeletalMesh,/Script/Engine.StaticMesh,/Script/Paper2D.PaperSprite,/Script/Paper2D.PaperFlipbook,/Script/PaperZD.PaperZDAnimSequence"))
    // clang-format on
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
};
#endif

template <>
struct POKEMONASSETS_API TBaseStructure<FBattleRender> {
    static UScriptStruct *Get();
};

template <>
struct POKEMONASSETS_API TBaseStructure<FSoftBattleRender> {
    static UScriptStruct *Get();
};

/**
 * Blueprint helper functions for a variant type.
 */
UCLASS()
class POKEMONASSETS_API UBattleRenderHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Variants, meta = (ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult MakeBattleRender(UObject *Object, FBattleRender &AsImageAsset);

    UFUNCTION(BlueprintPure, DisplayName = "Make Battle Render (SkeletalMesh)", Category = Variants,
              meta = (BlueprintAutocast))
    static FBattleRender MakeBattleRender_SkeletalMesh(USkeletalMesh *Mesh);

    UFUNCTION(BlueprintPure, DisplayName = "Make Battle Render (Static Mesh)", Category = Variants,
              meta = (BlueprintAutocast))
    static FBattleRender MakeBattleRender_StaticMesh(UStaticMesh *Mesh);

    UFUNCTION(BlueprintPure, DisplayName = "Make Battle Render (Material)", Category = Variants,
              meta = (BlueprintAutocast))
    static FBattleRender MakeBattleRender_PaperSprite(UPaperSprite *Sprite);

    UFUNCTION(BlueprintPure, DisplayName = "Make Battle Render (Paper Flipbook)", Category = Variants,
              meta = (BlueprintAutocast))
    static FBattleRender MakeBattleRender_PaperFlipbook(UPaperFlipbook *Flipbook);

    UFUNCTION(BlueprintPure, Category = Variants,
              meta = (CompactNodeTitle = "->", BlueprintAutocast, AutoCreateRefTerm = BattleRender))
    static UObject *ConvertToObject(const FBattleRender &BattleRender);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "BattleRender", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToSkeletalMesh(const FBattleRender &BattleRender, USkeletalMesh *&AsMesh);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "BattleRender", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToStaticMesh(const FBattleRender &BattleRender, UStaticMesh *&AsMesh);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "BattleRender", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToPaperSprite(const FBattleRender &BattleRender, UPaperSprite *&AsSprite);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = "BattleRender", ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult CastToPaperFlipbook(const FBattleRender &BattleRender, UPaperFlipbook *&AsFlipbook);

    UFUNCTION(BlueprintPure, Category = Variants,
              meta = (CompactNodeTile = "->", BlueprintAutocast, AutoCreateRefTerm = BattleRender))
    static FSoftBattleRender MakeSoftBattleRender(const FBattleRender &BattleRender);

    UFUNCTION(BlueprintCallable, Category = Variants, meta = (ExpandEnumAsExecs = "ReturnValue", AutoCreateRefTerm = Object))
    static EVariantFindResult MakeSoftBattleRenderFromSoftObjectPtr(const TSoftObjectPtr<UObject> &Object, FSoftBattleRender &AsSoftBattleRender);

    UFUNCTION(BlueprintCallable, Category = Variants,
              meta = (AutoCreateRefTerm = Path, ExpandEnumAsExecs = "ReturnValue"))
    static EVariantFindResult LoadSynchronous(const FSoftBattleRender &Path, FBattleRender &LoadedAsset);
};