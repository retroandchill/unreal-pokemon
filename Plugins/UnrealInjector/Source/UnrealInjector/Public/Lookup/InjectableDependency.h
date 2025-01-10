// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DependencyInjectionSettings.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Concepts/Interfaces.h"
#include "RetroLib/Optionals/OrElseGet.h"
#include "RetroLib/Ranges/Algorithm/FindFirst.h"

#include "RetroLib/Ranges/Views/NameAliases.h"
#include "UObject/Interface.h"

namespace UnrealInjector {

    template <typename T>
    concept CanInject = std::derived_from<T, UObject> || Retro::UnrealInterface<T>;

    template <typename T>
        requires CanInject<T>
    class TInjectionSettings {};

    template <typename T>
        requires CanInject<T>
    class TInjectableDependency;

    template <typename T>
    concept Injectable = CanInject<T> && requires {
        { TInjectionSettings<T>::Get() } -> std::same_as<const TInjectableDependency<T> &>;
    };

    template <typename T, typename... A>
    concept InitializableFrom = requires(T Injection, A &&...Args) { Injection.Initialize(std::forward<A>(Args)...); };

    template <typename T, typename... A>
    concept CanInitialize = (!InitializableFrom<T> && sizeof...(A) == 0) || InitializableFrom<T, A...>;

    template <typename T>
        requires CanInject<T>
    class TInjectableDependency {
      public:
        TInjectableDependency() {
            FCoreDelegates::OnPostEngineInit.Add(
                FSimpleDelegate::CreateRaw(this, &TInjectableDependency::SetUpInjection));
        }

        ~TInjectableDependency() = default;
        TInjectableDependency(const TInjectableDependency &) = delete;
        TInjectableDependency &operator=(const TInjectableDependency &) = delete;
        TInjectableDependency(TInjectableDependency &&) = delete;
        TInjectableDependency &operator=(TInjectableDependency &&) = delete;

        template <typename... A>
            requires CanInitialize<T, A...>
        auto Inject(UObject *Outer, A &&...Args) const {
            auto CreatedObject = CreateInjection(Outer, ClassPtr.LoadSynchronous());
            if constexpr (InitializableFrom<T, A...>) {
                CreatedObject->Initialize(std::forward<A>(Args)...);
            }
            return CreatedObject;
        }

      private:
        void SetUpInjection() {
            auto Setting = GetMutableDefault<UDependencyInjectionSettings>();
            // clang-format off
            auto& Result = Setting->TargetInjections |
                Retro::Ranges::Views::Filter([](const FInjectionTarget& Target) { return Retro::TypesMatch<T>(Target.TargetInterface); }) |
                Retro::Ranges::FindFirst() |
                Retro::Optionals::OrElseGet([Setting]() -> auto& { return Setting->TargetInjections.Emplace_GetRef(Retro::GetClass<T>()); });
            // clang-format on

            ClassPtr = Result.InjectedClass;
#if WITH_EDITOR
            Setting->TryUpdateDefaultConfigFile();
            Result.OnInjectedClassEdited.AddLambda([this](const FSoftClassPath &Path) { ClassPtr = Path; });
#endif
        }

        static auto CreateInjection(UObject *Outer, const UClass *InjectedClass) {
            if constexpr (std::derived_from<T, UObject>) {
                return NewObject<T>(Outer, InjectedClass);
            } else {
                return TScriptInterface<T>(NewObject<UObject>(Outer, InjectedClass));
            }
        }

        TSoftClassPtr<> ClassPtr;
    };

} // namespace UnrealInjector

#define DECLARE_INJECTABLE_DEPENDENCY(Export, Type)                                                                    \
    namespace UnrealInjector {                                                                                         \
        template <>                                                                                                    \
        class Export TInjectionSettings<Type> {                                                                        \
          public:                                                                                                      \
            static const TInjectableDependency<Type> &Get();                                                           \
                                                                                                                       \
          private:                                                                                                     \
            static TInjectableDependency<Type> InjectedDependency;                                                     \
        };                                                                                                             \
    }

#define DEFINE_INJECTABLE_DEPENDENCY(Type)                                                                             \
    namespace UnrealInjector {                                                                                         \
        TInjectableDependency<Type> TInjectionSettings<Type>::InjectedDependency;                                      \
        const TInjectableDependency<Type> &TInjectionSettings<Type>::Get() {                                           \
            return InjectedDependency;                                                                                 \
        }                                                                                                              \
    }