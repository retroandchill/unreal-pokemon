// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Types.h"
#include "Utilities.h"
#include "Ranges/Utilities/ForwardLike.h"

namespace UE::Optionals {

    template <typename F>
    struct TMapInvoker {
        template <typename T>
            requires std::constructible_from<F, T>
        explicit constexpr TMapInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
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
            using ContainedType = decltype(*Optional);
            if constexpr (std::is_lvalue_reference_v<TContainedOptionalType<O>> &&
                          std::invocable<F, TNullableValue<O>>) {
                using ResultType =
                    TOptionalType<decltype(ranges::invoke(Functor, GetNullableValue<O>(std::forward<O>(Optional))))>;
                return Optional.IsSet() ? ranges::invoke(Functor, GetNullableValue<O>(std::forward<O>(Optional)))
                                        : TOptional<ResultType>();
            } else if constexpr (std::is_lvalue_reference_v<TContainedOptionalType<O>>) {
                using ResultType =
                    TOptionalType<decltype(ranges::invoke(Functor, *Optional))>;
                return Optional.IsSet() ? ranges::invoke(Functor, *Optional)
                                        : TOptional<ResultType>();
            } else {
                using ResultType = TOptionalType<decltype(ranges::invoke(Functor, Ranges::ForwardLike<O&&, ContainedType>(*Optional)))>;
                return Optional.IsSet() ? ranges::invoke(Functor, Ranges::ForwardLike<O&&, ContainedType>(*Optional)) : TOptional<ResultType>();
            }
        }

      private:
        F Functor;
    };

    struct FMap {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = std::decay_t<decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...))>;
            return TOptionalClosure<TMapInvoker<BindingType>>(
                TMapInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    constexpr FMap Map;

} // namespace UE::Optionals
