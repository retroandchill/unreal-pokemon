// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PropertyTraits.h"

namespace Blueprint::Globals {
    class IPropertyTrait {
    public:
        virtual ~IPropertyTrait() = default;
    };

    template <typename T>
        requires HasPropertyType<T>
    class TPropertyTraitImpl : public IPropertyTrait {
    public:
        
    };

    class BLUEPRINTGLOBALVARIABLES_API FPropertyTraitRegistry {
        FPropertyTraitRegistry() = default;
        ~FPropertyTraitRegistry() = default;

    public:
        UE_NONCOPYABLE(FPropertyTraitRegistry)
        
        static FPropertyTraitRegistry &Get();

        template <typename T>
            requires HasPropertyType<T>
        void AddPrimitiveProperty() {
            PropertyTraits.Add(MakeUnique<TPropertyTraitImpl<T>>());
        }

    private:
        TMap<FName, TUniquePtr<IPropertyTrait>> PropertyTraits;
    };

    template <typename T>
        requires HasPropertyType<T>
    bool RegisterPrimitiveProperty() {
        FCoreDelegates::OnPostEngineInit.AddLambda([] {
            FPropertyTraitRegistry::Get().AddPrimitiveProperty<T>();
        });
        return true;
    }
}