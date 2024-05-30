// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALINJECTOR_API IRegisteredInjection {
public:
    virtual ~IRegisteredInjection();

    virtual UClass* GetInterfaceClass() const = 0;

    virtual FText GetDisplayName() const = 0;

    virtual UClass* GetFirstInjectableClass() const = 0;
    
    virtual TArray<UClass*> GetAllInjectableClasses() const = 0;
};
