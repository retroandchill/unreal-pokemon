// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Traits.h"

namespace UE::Optionals {

    template <typename F>
    struct TOrInvoker {
        explicit constexpr TOrInvoker(F &&Functor) : Functor(Functor) {
        }

        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional) const {
            return Optional.IsSet() ? Optional : Functor();
        }

      private:
        F Functor;
    };

    struct FOr {

        template <typename... A>
            requires Ranges::CanCreateBinding<A...>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...));
            return TOptionalClosure<TOrInvoker<BindingType>>(
                TOrInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    constexpr FOr Or;

} // namespace UE::Optionals
