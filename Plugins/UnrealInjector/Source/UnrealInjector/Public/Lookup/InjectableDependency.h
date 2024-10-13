// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DependencyInjectionSettings.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/Filter.h"
#include "UObject/Interface.h"

namespace UnrealInjector {

    template <typename T>
    concept CanInject = std::derived_from<T, UObject> || UE::Ranges::UnrealInterface<T>;

    template <typename T>
        requires CanInject<T>
    class TInjectionSettings {};

    template <typename T>
        requires CanInject<T>
    class TInjectableDependency;

    template <typename T>
    concept Injectable = CanInject<T> && requires {
        { TInjectionSettings<T>::Get() } -> std::same_as<const TInjectableDependency<T>&>;
    };

    template <typename T, typename... A>
    concept InitializableFrom = requires(T Injection, A&&... Args) {
        Injection.Initialize(Forward<A>(Args)...);
    };

    template <typename T, typename... A>
    concept CanInitialize = (!InitializableFrom<T> && sizeof...(A) == 0) || InitializableFrom<T, A...>;
    
    template <typename T>
        requires CanInject<T>
    class TInjectableDependency {
    public:
        TInjectableDependency() {
            FCoreDelegates::OnPostEngineInit.Add(FSimpleDelegate::CreateRaw(this, &TInjectableDependency::SetUpInjection));
        }

        ~TInjectableDependency() = default;
        TInjectableDependency(const TInjectableDependency&) = delete;
        TInjectableDependency& operator=(const TInjectableDependency&) = delete;
        TInjectableDependency(TInjectableDependency&&) = delete;
        TInjectableDependency& operator=(TInjectableDependency&&) = delete;

        template <typename... A>
            requires CanInitialize<T, A...>
        auto Inject(UObject* Outer, A&&... Args) const {
            auto CreatedObject = CreateInjection(Outer, ClassPtr.LoadSynchronous());
            if constexpr (InitializableFrom<T, A...>) {
                CreatedObject->Initialize(Forward<A>(Args)...);
            }
            return CreatedObject;
        }
        

    private:
        void SetUpInjection() {
            auto Setting = GetMutableDefault<UDependencyInjectionSettings>();
            // clang-format off
            auto& Result = Setting->TargetInjections |
                UE::Ranges::Filter([](const FInjectionTarget& Target) { return UE::Ranges::TypesMatch<T>(Target.TargetInterface); }) |
                UE::Ranges::FindFirst |
                UE::Optionals::OrElseGet([Setting]() -> auto& { return Setting->TargetInjections.Emplace_GetRef(UE::Ranges::GetClass<T>()); });
            // clang-format on

            ClassPtr = Result.InjectedClass;
#if WITH_EDITOR
            Setting->TryUpdateDefaultConfigFile();
            Result.OnInjectedClassEdited.AddLambda([this](const FSoftClassPath& Path) {
                ClassPtr = Path;
            });
#endif
        }
        
        static auto CreateInjection(UObject* Outer, const UClass* InjectedClass) {
            if constexpr (std::derived_from<T, UObject>) {
                return NewObject<T>(Outer, InjectedClass);
            } else {
                return TScriptInterface<T>(NewObject<UObject>(Outer, InjectedClass));
            }
        }
        
        TSoftClassPtr<> ClassPtr;
    };

        
}

#define DECLARE_INJECTABLE_DEPENDENCY(Export, Type) \
    namespace UnrealInjector { \
    template <> \
    class Export TInjectionSettings<Type> { \
        public: \
            static const TInjectableDependency<Type>& Get(); \
        private: \
            static TInjectableDependency<Type> InjectedDependency; \
    }; \
    }

#define DEFINE_INJECTABLE_DEPENDENCY(Type) \
    namespace UnrealInjector { \
    TInjectableDependency<Type> TInjectionSettings<Type>::InjectedDependency; \
    const TInjectableDependency<Type>& TInjectionSettings<Type>::Get() { \
        return InjectedDependency; \
    } \
    }