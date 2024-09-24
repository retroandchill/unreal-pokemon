// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Tuples.h"

namespace UE::Ranges {

    template <typename F>
    class TFunctionalBinding {

    public:
        explicit constexpr TFunctionalBinding(F&& Functor) : Functor(Forward<F>(Functor)) {}

        template <typename... A>
            requires std::invocable<F, A...>
        constexpr auto operator()(A&&... Args) {
            return std::invoke(Functor, Forward<A>(Args)...);
        }

        template <typename U>
            requires CanApply<U, F>
        constexpr auto operator()(U&& Args) {
            return std::apply(Functor, Forward<U>(Args));
        }
        
    private:
        F Functor;
    };

    template <typename F>
    auto WrapInvokable(F&& Functor) {
        return TFunctionalBinding<std::remove_cvref_t<F>>(Forward<F>(Functor));
    }
    
}