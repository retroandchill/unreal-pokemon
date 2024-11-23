// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Traits.h"

namespace UE::Optionals {

    template <typename F>
    struct TOrInvoker {
        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::remove_cvref_t<T>, TOrInvoker>)
        explicit constexpr TOrInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional) const {
            return Optional.IsSet() ? std::forward<O>(Optional) : Functor();
        }

      private:
        F Functor;
    };

    struct FOr {

        template <typename... A>
            requires Ranges::CanCreateBinding<A...>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = std::decay_t<decltype(Ranges::CreateBinding(std::forward<A>(Args)...))>;
            return TOptionalClosure<TOrInvoker<BindingType>>(
                TOrInvoker<BindingType>(Ranges::CreateBinding(std::forward<A>(Args)...)));
        }
    };

    constexpr FOr Or;

} // namespace UE::Optionals
