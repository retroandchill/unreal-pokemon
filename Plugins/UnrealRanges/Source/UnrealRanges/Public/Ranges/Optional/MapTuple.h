﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Concepts/Tuples.h"
#include "Ranges/Functional/Bindings.h"
#include "Types.h"

namespace UE::Optionals {

    template <typename F>
    struct TMapTupleInvoker {
        explicit constexpr TMapTupleInvoker(F &&Functor) : Functor(MoveTemp(Functor)) {
        }

        /**
         * Map the optional to a new value if present, otherwise return an empty optional
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The mapped optional
         */
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional) const {
            using ResultType = TOptionalType<decltype(ranges::tuple_apply(Functor, *Optional))>;
            return Optional.IsSet() ? ranges::tuple_apply(Functor, *Optional) : TOptional<ResultType>();
        }

      private:
        F Functor;
    };

    struct FMapTuple {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(Forward<A>(Args)...));
            return TOptionalClosure<TMapTupleInvoker<BindingType>>(
                TMapTupleInvoker<BindingType>(Ranges::CreateBinding<A...>(Forward<A>(Args)...)));
        }
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    constexpr FMapTuple MapTuple;
} // namespace UE::Optionals
