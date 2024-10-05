// "Unreal Pokémon" created by Retro & Chill.


#include "Images/ImageAsset.h"

static UScriptStruct* StaticGetBaseStructureInternal(FName Name) {
    static UPackage* CoreUObjectPkg = FindObjectChecked<UPackage>(nullptr, TEXT("/Script/RPGMenus"));
    UScriptStruct* Result = static_cast<UScriptStruct *>(StaticFindObjectFastInternal(UScriptStruct::StaticClass(),
        CoreUObjectPkg, Name, false, RF_NoFlags, EInternalObjectFlags::None));
    check(Result != nullptr);
    return Result;
}

UScriptStruct * TBaseStructure<FImageAsset>::Get() {
    static UScriptStruct* ScriptStruct = StaticGetBaseStructureInternal(TEXT("ImageAsset"));
    return ScriptStruct;
}