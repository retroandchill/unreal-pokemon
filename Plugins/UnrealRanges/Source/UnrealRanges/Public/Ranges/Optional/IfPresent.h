﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Traits.h"

namespace UE::Optionals {

    template <typename F>
    struct TIfPresentInvoker {
        explicit constexpr TIfPresentInvoker(F &&Functor) : Functor(std::move(Functor)) {
        }

        /**
         * Perform the action of the functor if the value is present inside the optional.
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The mapped optional
         */
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional) const {
            if (Optional.IsSet()) {
                ranges::invoke(Functor, *Optional);
            }
        }

      private:
        F Functor;
    };

    struct FIfPresent {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...));
            return TOptionalClosure<TIfPresentInvoker<BindingType>>(
                TIfPresentInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    constexpr FIfPresent IfPresent;

} // namespace UE::Optionals