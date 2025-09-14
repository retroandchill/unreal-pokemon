// "Unreal Pokémon" created by Retro & Chill.

#include "Images/ImageAsset.h"
#include "RetroLib/Variants/VariantObjectStruct.h"

RETRO_DEFINE_VARIANT_OBJECT_STRUCT(FImageAsset);

static UScriptStruct *StaticGetBaseStructureInternal(FName Name)
{
    static const auto *const CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/RPGMenus"));
    auto Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(
        UScriptStruct::StaticClass(), CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr)
#if WITH_EDITOR
    Result->SetMetaData(TEXT("VariantObject"), TEXT("true"));
#endif
    return Result;
}

UScriptStruct *TBaseStructure<FImageAsset>::Get()
{
    static auto *ScriptStruct = StaticGetBaseStructureInternal(TEXT("ImageAsset"));
    return ScriptStruct;
}

UScriptStruct *TBaseStructure<FSoftImageAsset>::Get()
{
    static UScriptStruct *ScriptStruct = StaticGetBaseStructureInternal(TEXT("SoftImageAsset"));
    return ScriptStruct;
}