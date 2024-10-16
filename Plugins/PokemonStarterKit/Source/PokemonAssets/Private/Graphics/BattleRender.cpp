// "Unreal Pokémon" created by Retro & Chill.


#include "Graphics/BattleRender.h"

UE_DEFINE_VARIANT_OBJECT_STRUCT(FBattleRender);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name) {
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/PokemonAssets"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
    Result->SetMetaData(TEXT("VariantObject"), TEXT("true"));
    return Result;
}

UScriptStruct * TBaseStructure<FBattleRender>::Get() {
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("BattleRender"));
    return ScriptStruct;
}

UScriptStruct * TBaseStructure<FSoftBattleRender>::Get() {
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftBattleRender"));
    return ScriptStruct;
}

EVariantFindResult UBattleRenderHelpers::MakeBattleRender(UObject *Object, FBattleRender &AsImageAsset) {
    if (!FBattleRender::GetTypeIndex(Object).IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    AsImageAsset.Set(Object);
    return EVariantFindResult::CastSucceeded;
}

FBattleRender UBattleRenderHelpers::MakeBattleRender_SkeletalMesh(USkeletalMesh *Mesh) {
    return FBattleRender(Mesh);
}

FBattleRender UBattleRenderHelpers::MakeBattleRender_StaticMesh(UStaticMesh *Mesh) {
    return FBattleRender(Mesh);
}

FBattleRender UBattleRenderHelpers::MakeBattleRender_PaperSprite(UPaperSprite *Sprite) {
    return FBattleRender(Sprite);
}

FBattleRender UBattleRenderHelpers::MakeBattleRender_PaperFlipbook(UPaperFlipbook *Flipbook) {
    return FBattleRender(Flipbook);
}

UObject * UBattleRenderHelpers::ConvertToObject(const FBattleRender &BattleRender) {
    return BattleRender.TryGet().GetPtrOrNull();
}

EVariantFindResult UBattleRenderHelpers::CastToSkeletalMesh(const FBattleRender &BattleRender, USkeletalMesh *&AsMesh) {
    if (!BattleRender.IsType<USkeletalMesh>()) {
        return EVariantFindResult::CastFailed;
    }

    AsMesh = &BattleRender.Get<USkeletalMesh>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UBattleRenderHelpers::CastToStaticMesh(const FBattleRender &BattleRender, UStaticMesh *&AsMesh) {
    if (!BattleRender.IsType<UStaticMesh>()) {
        return EVariantFindResult::CastFailed;
    }

    AsMesh = &BattleRender.Get<UStaticMesh>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UBattleRenderHelpers::CastToPaperSprite(const FBattleRender &BattleRender, UPaperSprite *&AsSprite) {
    if (!BattleRender.IsType<UPaperSprite>()) {
        return EVariantFindResult::CastFailed;
    }

    AsSprite = &BattleRender.Get<UPaperSprite>();
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UBattleRenderHelpers::CastToPaperFlipbook(const FBattleRender &BattleRender, UPaperFlipbook *&AsFlipbook) {
    if (!BattleRender.IsType<UPaperFlipbook>()) {
        return EVariantFindResult::CastFailed;
    }

    AsFlipbook = &BattleRender.Get<UPaperFlipbook>();
    return EVariantFindResult::CastSucceeded;
}

FSoftBattleRender UBattleRenderHelpers::MakeSoftBattleRender(const FBattleRender &BattleRender) {
    return FSoftBattleRender(BattleRender);
}

EVariantFindResult UBattleRenderHelpers::MakeSoftBattleRenderFromSoftObjectPtr(const TSoftObjectPtr<UObject> &Object,
    FSoftBattleRender &AsSoftBattleRender) {
    if (!FSoftBattleRender::GetTypeIndex(Object).IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    AsSoftBattleRender.Set(Object);
    return EVariantFindResult::CastSucceeded;
}

EVariantFindResult UBattleRenderHelpers::LoadSynchronous(const FSoftBattleRender &Path, FBattleRender &LoadedAsset) {
    auto Result = Path.LoadSynchronous();
    if (!Result.IsSet()) {
        return EVariantFindResult::CastFailed;
    }

    LoadedAsset = FBattleRender(*Result);
    return EVariantFindResult::CastSucceeded;
}