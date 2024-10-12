// "Unreal Pokémon" created by Retro & Chill.

#include "Lookup/InjectionUtilities.h"

TOptional<UClass&> UnrealInjector::GetFirstInjectableObject(const UClass *Class) {
    
    for (TObjectIterator<UClass> It; It; ++It) {
        if ((It->ImplementsInterface(Class) || It->IsChildOf(Class)) && !It->HasAnyClassFlags(CLASS_Abstract)) {
            return *It;
        }
    }

    return nullptr;
}