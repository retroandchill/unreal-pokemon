// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/BattleRender.h"

RETRO_DEFINE_VARIANT_OBJECT_STRUCT(FBattleRender);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name) {
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/PokemonAssets"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
    Result->SetMetaData(TEXT("VariantObject"), TEXT("true"));
    return Result;
}

UScriptStruct *TBaseStructure<FBattleRender>::Get() {
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("BattleRender"));
    return ScriptStruct;
}

UScriptStruct *TBaseStructure<FSoftBattleRender>::Get() {
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftBattleRender"));
    return ScriptStruct;
}