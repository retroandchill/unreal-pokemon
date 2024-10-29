// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Blueprints {
    class IVariableTypeRegistration;


    class BLUEPRINTGLOBALVARIABLES_API FRegistry {
        FRegistry() = default;
        ~FRegistry() = default;

    public:
        static FRegistry& Get();

    private:
        static FRegistry Instance;
        TMap<FName, TUniquePtr<IVariableTypeRegistration>> RegisteredTypes;
    };
    
    
}
