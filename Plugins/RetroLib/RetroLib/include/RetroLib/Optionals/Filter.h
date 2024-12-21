/**
 * @file Filter.h
 * @brief Closure for filtering an optional value
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    /**
     * @brief Performs a filter operation on an optional-like object using a provided set of arguments.
     *
     * This function evaluates whether the given optional-like object satisfies the filter criteria
     * defined by the functor or callable object constructed from the provided arguments. If the
     * criteria are met, the function returns an optional or optional-like containing the value.
     * Otherwise, an empty optional or optional-like is returned.
     *
     * @tparam O The type of the optional-like object to be filtered. Can be an lvalue or rvalue reference.
     * @tparam A Variadic template parameter representing the types of arguments passed to construct the callable
     * object.
     * @param optional The optional-like object to evaluate and potentially filter.
     * @param args Variadic arguments used to construct the functor or callable object for the filter operation.
     * @return For lvalue references, a filtered optional-like object using `make_optional_reference`.
     *         For rvalue references, a filtered optional-like object containing a copy or an empty instance.
     *
     * @note The operation evaluates `has_value` on the optional-like object and applies the callable
     *       to the value if present.
     * @note The callable object must be invocable with the value type of the optional-like object
     *       and return a boolean indicating whether the value satisfies the filter condition.
     */
    RETROLIB_EXPORT template <OptionalType O, typename... A>
        requires std::is_invocable_r_v<bool, BindingType<A...>, CommonReference<O>>
    constexpr auto filter(O &&optional, A &&...args) {
        if constexpr (std::is_lvalue_reference_v<O>) {
            using FilteredType = decltype(make_optional_reference(std::forward<O>(optional)));
            return has_value(std::forward<O>(optional)) &&
                           std::invoke(create_binding(std::forward<A>(args)...), get<O>(std::forward<O>(optional)))
                       ? make_optional_reference(std::forward<O>(optional))
                       : FilteredType();
        } else {
            return has_value(std::forward<O>(optional)) &&
                           std::invoke(create_binding(std::forward<A>(args)...), get<O>(std::forward<O>(optional)))
                       ? O(std::forward<O>(optional))
                       : O();
        }
    }

    /**
     * Filters the given optional based on a functor condition.
     *
     * This function checks if the optional has a value and if the functor condition is satisfied.
     * If both are true, it returns the original optional; otherwise, it returns an empty optional.
     * This is performed differently for lvalue and rvalue references.
     *
     * @tparam O The type of the optional being filtered.
     * @param optional The optional value to filter. This can be an lvalue or rvalue reference.
     * @param args The binding arguments for the functor
     * @return A filtered optional, which contains its original value if the condition is satisfied, or is empty
     * otherwise.
     */
    RETROLIB_EXPORT template <auto Functor, OptionalType O, typename... A>
        requires std::is_invocable_r_v<bool, BindingType<decltype(Functor), A...>, CommonReference<O>>
    constexpr auto filter(O &&optional, A &&...args) {
        return filter(std::forward<O>(optional), create_binding<Functor>(std::forward<A>(args)...));
    }

    /**
     * @brief The FilterInvoker struct provides a callable object that applies a filter operation
     *        on an optional-like object using a provided functor.
     *
     * This struct represents a function object that can be used to filter values
     * contained within an optional object. The filtering is done according to the
     * criteria defined by the provided functor, which must be invocable and return
     * a boolean value.
     *
     * @note The functor must be a callable object that takes a CommonReference of type O
     *       and returns a boolean. The filter operation is only applied if O contains a value.
     */
    struct FilterInvoker {
        /**
         * @brief Invokes the filter operation on an optional-like object using a given functor.
         *
         * This function applies the provided functor to filter values within the supplied
         * optional-like object. The functor determines whether the contained value should be
         * accepted based on its implemented criteria.
         *
         * @param optional The optional-like object that may contain a value to be filtered.
         *                 It is forwarded to allow modification or observation.
         * @param args The callable binding parameters used as a predicate to evaluate the contained value.
         *                This must be an invocable object that returns a boolean indicating whether
         *                the value meets the criteria for inclusion.
         *
         * @return The result of the filter operation, which is either the original optional-like
         *         object (if it passes the filter) or a modified state that reflects the absence
         *         of a valid value, depending on the outcome of the functor.
         */
        template <OptionalType O, typename... A>
            requires std::is_invocable_r_v<bool, BindingType<A...>, CommonReference<O>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return filter(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    /**
     * @brief A constant instance of FilterInvoker that provides a mechanism to apply filter operations
     *        on optional-like objects using a specified functor.
     *
     * The filter_invoker is a predefined, immutable instance of the FilterInvoker struct,
     * enabling filtering of values in optional-like containers based on the criteria
     * specified by the functor. The functor determines whether a value meets the desired
     * condition.
     *
     * @note This constant can be utilized to perform filter operations without needing
     *       to instantiate a new FilterInvoker object each time a filter needs to be applied.
     */
    constexpr FilterInvoker filter_invoker;

    /**
     * @brief The ConstFilterInvoker struct serves as a callable object to invoke a filter operation
     *        with a constant functor on an optional-like object.
     *
     * This struct defines a function object designed to filter values within an optional object
     * using a predefined functor. The functor is assumed to be constant and is employed to
     * determine if a value meets certain conditions, thus filtering the optional content.
     *
     * @note The callable requires the functor, referred to by Functor, to be invocable
     *       and return a boolean when applied to a CommonReference of type O.
     *       The condition is only evaluated if O contains an actual value.
     */
    template <auto Functor>
    struct ConstFilterInvoker {
        /**
         * @brief Applies the filter operation on a forwarded optional-like object.
         *
         * This function call operator allows the struct to be invoked as a callable
         * that performs a filter operation on an optional-like object using a predefined
         * functor. The filtering decision is based on the criteria set by the functor.
         *
         * @param optional The optional-like object to be filtered. It is expected to be
         *                 an rvalue reference that will be perfectly forwarded.
         * @param args The binding arguments for the functor
         *
         * @return The result of invoking the filter operation on the provided optional-like
         *         object, with the filter's functor determining whether any contained value
         *         meets the criteria.
         */
        template <OptionalType O, typename... A>
            requires std::is_invocable_r_v<bool, BindingType<decltype(Functor), A...>, CommonReference<O>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return filter<Functor>(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    /**
     * @brief The const_filter_invoker is an instance of the ConstFilterInvoker class template
     *        parameterized with a specific functor type.
     *
     * This instance serves as a constant, callable object that performs a filter operation
     * on a constant optional-like object. The filtering mechanism relies on the provided
     * functor, which needs to be callable and should return a boolean value to determine
     * whether a value should be retained or not.
     *
     * @note The template parameter, Functor, must be invocable with a CommonReference of type O
     *       and is expected to return a boolean. The filter operation will be performed only
     *       if the opted object holds a value.
     */
    template <auto Functor>
    constexpr ConstFilterInvoker<Functor> const_filter_invoker;

    /**
     * @brief Creates an extension method that applies a filter operation using the provided functor.
     *
     * This function generates an extension method by forwarding the provided functor
     * to the filter_invoker. The functor is used to define the criteria for filtering,
     * which must be invocable and return a boolean value, indicating whether an element
     * should be included or not.
     *
     * @param args A callable binding parameters that takes an element and returns a boolean.
     *                This functor defines the criteria for filtering elements.
     *
     * @return An extension method that applies the filter operation defined by the functor.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr auto filter(A &&...args) {
        return extension_method<filter_invoker>(std::forward<A>(args)...);
    }

    /**
     * @brief Returns an extension method that applies a const filter invoker using the specified functor.
     *
     * This function constructs and returns an extension method encapsulated by a const_filter_invoker
     * utilizing the provided functor. The functor is expected to perform a filtering operation,
     * applying its criteria on input values to determine whether they should be retained.
     *
     * @param args The binding arguments for the functor
     * @return An extension method configured with a const_filter_invoker that uses the given functor
     *         for filtering operations.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto filter(A &&...args) {
        return extension_method<const_filter_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals