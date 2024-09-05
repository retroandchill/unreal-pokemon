// "Unreal Pokémon" created by Retro & Chill.


#include "Storage/StorageBoxDTO.h"
#include "Lookup/InjectionUtilities.h"
#include "Storage/StorageBox.h"


TScriptInterface<IStorageBox> FStorageBoxDTO::CreateBox(UObject *Outer) const {
    return UnrealInjector::NewInjectedDependency<IStorageBox>(Outer, *this);
}