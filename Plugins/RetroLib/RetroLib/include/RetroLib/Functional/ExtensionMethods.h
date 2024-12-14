/**
 * @file ExtensionMethods.h
 * @brief Contains support for extension methods that can be created and invoked on an variable using the pipe (|)
 * operator.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/WrapArg.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * @brief A struct that allows extension of methods using a provided functor.
     *
     * The `ExtensionMethod` struct template utilizes a given functor to provide additional
     * functionality through extension methods. The functor is required to have a function
     * call operator, enforced via the `HasFunctionCallOperator` concept.
     *
     * @tparam Functor A callable object that meets the `HasFunctionCallOperator` requirement.
     *
     * The struct provides an `operator()` that mimics the call signature of the provided
     * functor. It accepts any arguments that are invocable with the functor and forwards
     * them to the functor, returning the result.
     *
     * The `operator()` ensures that any valid call to the functor can be performed
     * through an instance of this struct, leveraging parameter forwarding.
     */
    template <auto Functor>
        requires(is_valid_functor_object(Functor))
    struct ExtensionMethod {

        /**
         * @brief Invokes the stored functor with the provided arguments.
         *
         * This function call operator allows an instance of the struct to act as if it
         * were the functor itself, accepting any arguments that the functor can take.
         * It forwards these arguments to the functor using perfect forwarding to maintain
         * their value category and returns the result.
         *
         * @tparam A The types of the arguments passed to the functor.
         * @param args The arguments to forward to the functor. These arguments will be perfectly
         * forwarded to preserve their original types, whether they are lvalue or rvalue references.
         * @return The result of invoking the functor with the specified arguments.
         *
         * @note This function can only be used if the functor is invocable with the provided arguments,
         * which is enforced by the requires clause.
         */
        template <typename... A>
            requires std::invocable<decltype(Functor), A...>
        constexpr decltype(auto) operator()(A &&...args) const {
            return std::invoke(Functor, std::forward<A>(args)...);
        }
    };

    /**
     * @brief Trait to determine if a type is an extension method.
     *
     * This primary template declares the type as not being an extension method
     * by default, inheriting from std::false_type. Specializations of this struct
     * should be provided for actual extension method types, inheriting from
     * std::true_type.
     *
     * @tparam T The type to be checked.
     */
    template <typename>
    struct IsExtensionMethod : std::false_type {};

    /**
     * @brief Specialization to identify extension methods.
     *
     * This struct template checks if a given functor, wrapped in
     * an ExtensionMethod, qualifies as an extension method.
     * It inherits from std::true_type if the functor is an extension
     * method, effectively serving as a compile-time boolean constant
     * to represent this fact.
     *
     * @tparam Functor The functor type wrapped by the ExtensionMethod.
     *
     * @note This specialization only applies to functors that meet
     * the requirements specified by the HasFunctionCallOperator concept.
     */
    template <auto Functor>
        requires(is_valid_functor_object(Functor))
    struct IsExtensionMethod<ExtensionMethod<Functor>> : std::true_type {};

    /**
     * @brief Concept to determine if a type is an extension method.
     *
     * This concept checks if a given type satisfies the conditions defined by the IsExtensionMethod
     * trait. It evaluates to true if the IsExtensionMethod trait for the type T has a value of true,
     * indicating that T is an extension method type. Otherwise, it evaluates to false.
     *
     * @tparam T The type to be checked.
     */
    template <typename T>
    concept ExtensionMethodType = IsExtensionMethod<T>::value;

    /**
     * @brief A template structure that implements an extension method closure.
     *
     * This class is designed to encapsulate a callable entity (functor) and provide
     * functionality to apply this functor to operands using the pipe ('|') operator.
     * The class is templated on type F, which should satisfy the HasFunctionCallOperator
     * concept, meaning that it can be used as a callable function or a member function pointer.
     *
     * @tparam F The type of the functor that the closure will encapsulate. F should satisfy
     * the HasFunctionCallOperator concept, meaning it must be usable as a callable entity.
     *
     * @note This class is implemented with a constraint to ensure that recursive instances
     * of ExtensionMethodClosure are not constructed, and it requires that the functor is
     * convertible to the type F.
     */
    template <typename F>
        requires HasFunctionCallOperator<F>
    struct ExtensionMethodClosure {

        /**
         * @brief Constructs an ExtensionMethodClosure with a given callable entity.
         *
         * This constructor is explicit and constexpr, taking a perfect forwarded
         * parameter of type T. It initializes the functor in the closure using
         * std::forward to ensure perfect forwarding. This allows the encapsulation
         * of various callable types as long as they are convertible to the template
         * parameter F and do not themselves instantiate another ExtensionMethodClosure.
         *
         * @tparam T The type of the callable entity being passed. It should not be
         * the same as the decayed type of ExtensionMethodClosure and must be convertible
         * to the functor type F.
         *
         * @param functor A callable entity to be encapsulated by the closure. It is
         * forwarded perfectly, preserving its value category and const/volatile
         * qualifiers.
         */
        template <typename T>
            requires(!std::same_as<std::decay_t<T>, ExtensionMethodClosure>) && std::convertible_to<T, F>
        explicit constexpr ExtensionMethodClosure(T &&functor) : functor(std::forward<T>(functor)) {
        }

        /**
         * @brief Operator overload for the pipe ('|') operator to apply a functor to an operand.
         *
         * This operator enables using the pipe ('|') syntax to apply the functor encapsulated
         * within an ExtensionMethodClosure instance to a given operand.
         *
         * @tparam T The type of the operand being passed. It is perfectly forwarded to preserve
         * its value category and qualifiers.
         *
         * @param operand An instance of type T on which the function call is to be applied.
         * It is forwarded to maintain its original type characteristics.
         *
         * @param closure An ExtensionMethodClosure object that contains the functor to be invoked
         * with the operand.
         *
         * @return The result of invoking the encapsulated functor with the provided operand,
         * with the return type determined by the result of the functor call.
         */
        template <typename T>
            requires std::invocable<F, T>
        friend constexpr decltype(auto) operator|(T &&operand, const ExtensionMethodClosure &closure) {
            return std::invoke(closure.functor, std::forward<T>(operand));
        }

      private:
        F functor;
    };

    /**
     * ExtensionMethodConstClosure is a template structure that facilitates the use of
     * extension methods with a constant closure. It enables a functor to be used as an
     * extension method through operator overloading.
     *
     * The structure requires a valid functor object, which is validated through a custom
     * is_valid_functor_object constraint. This ensures the passed functor is callable.
     *
     * This feature allows the use of a functor in a pipeline style operation using the
     * bitwise OR operator, making it look like an extension method on the operand.
     *
     * Template Parameters:
     *   Functor - The callable object that will be used as an extension method.
     *
     * Supported Operations:
     *   - The operator|, which takes an operand and invokes the stored functor on it.
     */
    template <auto Functor>
        requires(is_valid_functor_object(Functor))
    struct ExtensionMethodConstClosure {
        /**
         * A friend constexpr function template that overloads the bitwise OR operator (|)
         * to enable a pipeline style operation that invokes the functor stored in
         * ExtensionMethodConstClosure on the given operand.
         *
         * This function leverages std::invoke to call the functor with the operand, allowing
         * for seamless integration of the extension method feature.
         *
         * Template Parameters:
         *   T - The type of the operand which the functor will be invoked upon.
         *
         * Parameters:
         *   operand - The operand of type T that will be passed to the functor.
         *   <unnamed> - An instance of ExtensionMethodConstClosure holding the functor to be invoked.
         *
         * Returns:
         *   The result of invoking the functor with the provided operand.
         */
        template <typename T>
            requires std::invocable<decltype(Functor), T>
        friend constexpr decltype(auto) operator|(T &&operand, const ExtensionMethodConstClosure &) {
            return std::invoke(Functor, std::forward<T>(operand));
        }
    };

    /**
     * @brief A structure for binding extension methods with a functor object.
     *
     * The ExtensionMethodBinder struct is utilized for creating closures
     * with a specified functor object. It binds additional parameters to
     * the functor, effectively allowing the delayed execution of the functor
     * with those bound arguments.
     *
     * @tparam Functor A compile-time constant representing the functor to bind.
     *                 Must fulfill the requirements of being a valid functor object.
     *
     * @requires Functor type must pass the `is_valid_functor_object` check, ensuring
     *           it has a function call operator and, if a pointer, it should not be null.
     *
     * @note This struct operates on a constexpr basis, allowing its functionality
     *       to be invoked during compile time if all arguments are compile-time constants.
     */
    template <auto Functor>
        requires(is_valid_functor_object(Functor))
    struct ExtensionMethodBinder {
        /**
         *
         * @brief This method enables the binding process with the specified functor.
         *
         * It provides two operational paths based on the presence of arguments:
         *
         * - When no arguments are provided, a constant closure is returned.
         *   This allows future invocations without additional parameters.
         *
         * - When one or more arguments are supplied, these are bound to the
         *   functor using `retro::bind_back`, forming an executable closure with
         *   predetermined arguments.
         *
         * @tparam T Variadic template parameter pack representing the types of
         *           arguments to bind to the functor.
         *
         * @param args Forwarding reference to the arguments to be bound with the functor.
         *             These are perfect-forwarded, preserving their value category.
         *
         * @return A callable closure with the provided arguments bound to the functor.
         *         This closure can be utilized to execute the functor at a later point.
         */
        template <typename... T>
        constexpr auto operator()(T &&...args) const {
            if constexpr (sizeof...(T) == 0) {
                return ExtensionMethodConstClosure<Functor>();
            } else {
                using BindingType = decltype(retro::bind_back<Functor>(wrap_arg<T>(std::forward<T>(args))...));
                return ExtensionMethodClosure<BindingType>(
                    retro::bind_back<Functor>(wrap_arg<T>(std::forward<T>(args))...));
            }
        }
    };

    RETROLIB_EXPORT template <auto Functor>
        requires(is_valid_functor_object(Functor))
    constexpr ExtensionMethodBinder<Functor> extension_method;
} // namespace retro