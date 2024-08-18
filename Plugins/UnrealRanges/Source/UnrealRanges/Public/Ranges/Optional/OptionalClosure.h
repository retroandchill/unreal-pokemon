// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Traits.h"

namespace UE::Optionals {

    /**
     * A range-based closure used to pipe optionals in order to chain manipulations, similar to how Optionals in Java work.
     * @tparam F The functor that is bound to the closure.
     */
    template <typename F>
    struct TOptionalClosure {

        constexpr TOptionalClosure() = default;

        explicit constexpr TOptionalClosure(F&& Functor) : Functor(MoveTemp(Functor)) {
        }

        template <typename O>
            requires UEOptional<O>
        friend constexpr auto operator|(O&& Optional, TOptionalClosure Closure) {
            return Closure.Functor(Forward<O>(Optional));
        }
    
    private:
        F Functor;
    };    
    
}
