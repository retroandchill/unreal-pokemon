// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "range/v3/functional/bind_back.hpp"
#include "Ranges/Concepts/Delegates.h"
#include "Ranges/Functional/DelegateBinding.h"
#include "Ranges/RangeConcepts.h"
#include <functional>

namespace UE::Ranges {

    /**
     * Bind the supplied functor with the rear-most arguments being bound to the specified values. This works similarly
     * to ranges::bind_back except that it can work with no arguments supplied, which will simply return the functor
     * back to the caller.
     * 
     * @tparam F The functor to bind back to.
     * @tparam A The types of the arguments
     * @param Functor The functor to bind
     * @param Args The arguments to bind
     * @return The bound functor
     */
    template <typename F, typename... A>
        requires FunctionalType<F> && (!UEDelegate<F>)
    constexpr decltype(auto) CreateBinding(F &&Functor, A &&...Args) {
        if constexpr (sizeof...(A) > 0) {
            return ranges::bind_back(std::forward<F>(Functor), std::forward<A>(Args)...);
        } else {
            return std::forward<F>(Functor);
        }
    }

    
    /**
     * Creates a binding of a functor with the specified arguments.
     *
     * Depending on whether the object type is derived from the class type
     * of the member function, it either binds the object reference or
     * the object itself.
     *
     * @tparam C The type of the object.
     * @tparam F The type of the functor (e.g., a member function).
     * @tparam A The types of the additional arguments.
     * @param Object The object to bind, either as a reference or as a value.
     * @param Functor The functor (e.g., member function) to be bound.
     * @param Args The additional arguments to be bound.
     * @return A callable entity that binds the functor with the specified arguments and object.
     */
    template <typename C, typename F, typename... A>
        requires StdExt::IsMemberFunction_v<F>
    constexpr decltype(auto) CreateBinding(C &&Object, F &&Functor, A &&...Args) {
        if constexpr (std::is_base_of_v<StdExt::MemberFunctionClass_t<F>, std::remove_cvref_t<C>>) {
            return std::bind_front(CreateBinding(std::forward<F>(Functor), std::forward<A>(Args)...), &Object);
        } else {
            return std::bind_front(CreateBinding(std::forward<F>(Functor), std::forward<A>(Args)...),
                                   std::forward<C>(Object));
        }
    }

    /**
     * Creates a binding for the specified member.
     *
     * @tparam M Type of the member.
     * @param Member The member to be bound.
     * @return The forward reference of the member.
     */
    template <typename M>
        requires std::is_member_pointer_v<M> && (!std::is_member_function_pointer_v<M>)
    constexpr decltype(auto) CreateBinding(M &&Member) {
        return std::forward<M>(Member);
    }

    /**
     * Creates a binding for a given delegate with the provided arguments.
     *
     * @tparam D The type of the delegate.
     * @tparam A The types of the arguments to bind to the delegate.
     * @param Delegate The delegate to bind the arguments to.
     * @param Args The arguments to bind to the delegate.
     * @return A callable entity that binds the delegate with the provided arguments.
     */
    template <typename D, typename... A>
        requires UEDelegate<D>
    constexpr decltype(auto) CreateBinding(D &&Delegate, A &&...Args) {
        return ranges::bind_back([Callback = std::forward<D>(Delegate)]<typename... B>(
                                     B &&...Vals) { return InvokeDelegate(Callback, std::forward<B>(Vals)...); },
                                 std::forward<A>(Args)...);
    }

    /**
     * Checks if the given parameters can be bound into a proper binding.
     *
     * @tparam A The parameters for the bindings
     */
    template <typename... A>
    concept CanCreateBinding = requires(A&&... Args) {
        { CreateBinding(std::forward<A>(Args)...) } -> FunctionalType;
    };

    /**
     * Checks if the given parameters can be bound into a proper binding and then invoked.
     *
     * @tparam T The type that will be passed into the binding
     * @tparam A The parameters for the bindings
     */
    template <typename T, typename... A>
    concept CanInvokeBinding = std::invocable<decltype(CreateBinding<A...>(std::forward<A>(std::declval<A>())...)), T>;

} // namespace UE::Ranges

/**
 * Shorthand for creating a wrapped reference value
 * 
 * @param Value The value to turn into a std::reference_wrapper instance.
 */
#define REF(Value) std::reference_wrapper(Value)