// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Types.h"

namespace UE::Optionals {

    template <typename F>
    struct TFilterInvoker {
        explicit constexpr TFilterInvoker(F &&Functor) : Functor(MoveTemp(Functor)) {
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
            using ResultType = std::remove_cvref_t<O>;
            return Optional.IsSet() && ranges::invoke(Functor, *Optional) ? Optional : ResultType();
        }

      private:
        F Functor;
    };

    struct FFilter {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(Ranges::CreateBinding<A...>(Forward<A>(Args)...));
            return TOptionalClosure<TFilterInvoker<BindingType>>(
                TFilterInvoker<BindingType>(Ranges::CreateBinding<A...>(Forward<A>(Args)...)));
        }
    };

    constexpr FFilter Filter;
} // namespace UE::Optionals
