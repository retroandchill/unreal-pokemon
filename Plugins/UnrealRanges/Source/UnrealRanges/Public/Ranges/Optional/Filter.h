// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Types.h"
#include "Ranges/Utilities/ForwardLike.h"

namespace UE::Optionals {

    template <typename F>
    struct TFilterInvoker {
        
        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::remove_cvref_t<T>, TFilterInvoker>)
        explicit constexpr TFilterInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        /**
         * Map the optional to a new value if present, otherwise return an empty optional
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The mapped optional
         */
        template <typename O>
            requires UEOptional<O>
        constexpr decltype(auto) operator()(O &&Optional) const {
            using ContainedType = decltype(*Optional);
            if constexpr (std::is_lvalue_reference_v<TContainedOptionalType<O>>) {
                using ResultType = std::remove_cvref_t<O>;
                return Optional.IsSet() && ranges::invoke(Functor, *Optional) ? ResultType(std::forward<O>(Optional)) : ResultType();
            } else if constexpr (SubclassOptional<O>) {
                using ResultType = TOptional<TOptionalElementType<O>>;
                return Optional.IsSet() && ranges::invoke(Functor, *Optional->Get()) ? ResultType(std::forward<O>(Optional)) : ResultType();
            } else if constexpr (std::is_lvalue_reference_v<O>) {
                using ResultType = TOptional<TOptionalElementType<O>&>;
                return Optional.IsSet() && ranges::invoke(Functor, Ranges::ForwardLike<O&&, ContainedType>(*Optional)) ? ResultType(*Optional) : ResultType();
            } else {
                using ResultType = std::remove_cvref_t<O>;
                return Optional.IsSet() && ranges::invoke(Functor, Ranges::ForwardLike<O&&, ContainedType>(*Optional)) ? ResultType(std::forward<O>(Optional)) : ResultType();
            }
        }

      private:
        F Functor;
    };

    struct FFilter {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = std::decay_t<decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...))>;
            return TOptionalClosure<TFilterInvoker<BindingType>>(
                TFilterInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    constexpr FFilter Filter;
} // namespace UE::Optionals
