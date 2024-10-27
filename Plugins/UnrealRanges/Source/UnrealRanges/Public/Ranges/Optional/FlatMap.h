// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Types.h"

namespace UE::Optionals {

    template <typename F>
    struct TFlatMapInvoker {
        explicit constexpr TFlatMapInvoker(F &&Functor) : Functor(std::move(Functor)) {
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
            using ResultType = TOptionalType<decltype(ranges::invoke(Functor, *Optional))>;
            return Optional.IsSet() ? ranges::invoke(Functor, *Optional) : ResultType();
        }

      private:
        F Functor;
    };

    struct FFlatMap {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...));
            return TOptionalClosure<TFlatMapInvoker<BindingType>>(
                TFlatMapInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    constexpr FFlatMap FlatMap;
} // namespace UE::Optionals