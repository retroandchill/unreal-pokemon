// "Unreal Pokémon" created by Retro & Chill.


#include "Lookup/InjectionUtilities.h"

UClass* UnrealInjector::GetFirstInjectableObject(TSubclassOf<UInterface> InterfaceType) {
    for (TObjectIterator<UClass> It; It; ++It) {
        if (It->ImplementsInterface(InterfaceType) && !It->HasAnyClassFlags(CLASS_Abstract)) {
            return *It;
        }
    }

    return nullptr;
}