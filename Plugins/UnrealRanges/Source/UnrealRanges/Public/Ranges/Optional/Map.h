// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Types.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Functional/Bindings.h"

namespace UE::Optionals {
        

    template <typename F>
    struct TMapInvoker {
        explicit constexpr TMapInvoker(F&& Functor) : Functor(MoveTemp(Functor)) {
        }

        /**
         * Map the optional to a new value if present, otherwise return an empty optional
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The mapped optional
         */
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O&& Optional) const {
            using ResultType = TOptionalType<decltype(ranges::invoke(Functor, *Optional))>;
            return Optional.IsSet() ? ranges::invoke(Functor, *Optional) : TOptional<ResultType>();
        }

    private:
        F Functor;
    };

    struct FMap {

        template <typename... A>
        constexpr auto operator()(A&&... Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(Forward<A>(Args)...));
            return TOptionalClosure<TMapInvoker<BindingType>>(TMapInvoker<BindingType>(
                Ranges::CreateBinding<A...>(Forward<A>(Args)...)));
        }
        
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    constexpr FMap Map;
}
