// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Traits.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Functional/Bindings.h"

namespace UE::Optionals {

    /**
     * Optional closure invocation used to invoke on an optional to get an alternate value
     * @tparam F The functor to invoke for alternate invocations
     */
    template <typename F>
    struct TOrElseGetInvoker {
        explicit constexpr TOrElseGetInvoker(F&& Functor) : Functor(Functor) {}

        /**
         * Obtain the underlying element of an optional, invoking the functor if nothing is present.
         * @tparam O 
         * @param Optional 
         * @return 
         */
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O&& Optional) const {
            using ResultType = TOptionalElementType<O>;
            return Optional.IsSet() ? *Optional : ResultType(Functor());
        }
        
    private:
        F Functor;
    };


    /**
     * Optional closure creator to get an alternate value out of an optional if necessary.
     */
    struct FOrElseGet {

        /**
         * Create the optional closure to operate with.
         * @tparam F The functor to bind
         * @tparam A The arguments to supply to the functor.
         * @param Functor The functor to bind
         * @param Args The arguments to supply to the functor.
         * @return The bound functional closure.
         */
        template <typename F, typename... A>
            requires Ranges::FunctionalType<F>
        constexpr auto operator()(F&& Functor, A&&... Args) const {
            return TOptionalClosure<TOrElseGetInvoker<F>>(TOrElseGetInvoker<F>(
                Ranges::BindFunctor<F, A...>(Forward<F>(Functor), Forward<A>(Args)...)));
        }
        
    };

    /**
     * Get the value of an optional, getting out the value as supplied by the contained functor if the value is not present.
     */
    constexpr FOrElseGet OrElseGet;
}
