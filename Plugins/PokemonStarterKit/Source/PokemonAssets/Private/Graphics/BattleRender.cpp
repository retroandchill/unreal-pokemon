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