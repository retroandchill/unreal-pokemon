// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RegisteredInjection.h"

/**
 * 
 */
template <typename T>
requires std::is_base_of_v<UInterface, T>
class TInterfaceInjection : public IRegisteredInjection {
public:
    UClass* GetInterfaceClass() const {
        return T::StaticClass();
    }
    
    FText GetDisplayName() const override {
        return T::StaticClass()->GetDisplayNameText();
    }

    UClass* GetFirstInjectableClass() const override {
        for (TObjectIterator<UClass> It; It; ++It) {
            if (It->ImplementsInterface(T::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract)) {
                return *It;
            }
        }
        
        return nullptr;
    }
    
    TArray<UClass*> GetAllInjectableClasses() const override {
        TArray<UClass*> Classes;
        for (TObjectIterator<UClass> It; It; ++It) {
            if (!It->ImplementsInterface(T::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract))
                continue;

            Classes.Add(*It);
        }
        return Classes;
    }
};
