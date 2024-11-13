// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Concepts/Tuples.h"
#include "Ranges/Functional/Bindings.h"
#include "Types.h"
#include "Ranges/Utilities/ForwardLike.h"

namespace UE::Optionals {

    template <typename F>
    struct TMapTupleInvoker {
        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::remove_cvref_t<T>, TMapTupleInvoker>)
        explicit constexpr TMapTupleInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
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
            using ResultType = TOptionalType<decltype(ranges::tuple_apply(Functor, Ranges::ForwardLike<O>(*Optional)))>;
            return Optional.IsSet() ? ranges::tuple_apply(Functor, Ranges::ForwardLike<O>(*Optional)) : TOptional<ResultType>();
        }

      private:
        F Functor;
    };

    struct FMapTuple {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = std::decay_t<decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...))>;
            return TOptionalClosure<TMapTupleInvoker<BindingType>>(
                TMapTupleInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    constexpr FMapTuple MapTuple;
} // namespace UE::Optionals
