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
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

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
        requires(IsValidFunctorObject(Functor))
    struct ExtensionMethodInvoker {

        /**
         * @brief Invokes the stored functor with the provided arguments.
         *
         * This function call operator allows an instance of the struct to act as if it
         * were the functor itself, accepting any arguments that the functor can take.
         * It forwards these arguments to the functor using perfect forwarding to maintain
         * their value category and returns the result.
         *
         * @tparam A The types of the arguments passed to the functor.
         * @param Args The arguments to forward to the functor. These arguments will be perfectly
         * forwarded to preserve their original types, whether they are lvalue or rvalue references.
         * @return The result of invoking the functor with the specified arguments.
         *
         * @note This function can only be used if the functor is invocable with the provided arguments,
         * which is enforced by the requires clause.
         */
        template <typename... A>
            requires std::invocable<decltype(Functor), A...>
        constexpr decltype(auto) operator()(A &&...Args) const {
            return std::invoke(Functor, std::forward<A>(Args)...);
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
        requires(IsValidFunctorObject(Functor))
    struct IsExtensionMethod<ExtensionMethodInvoker<Functor>> : std::true_type {};

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
         * @param Functor A callable entity to be encapsulated by the closure. It is
         * forwarded perfectly, preserving its value category and const/volatile
         * qualifiers.
         */
        template <typename T>
            requires(!std::same_as<std::decay_t<T>, ExtensionMethodClosure>) && std::convertible_to<T, F>
        explicit constexpr ExtensionMethodClosure(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        /**
         * Applies the stored functor to the given operand parameter.
         *
         * This operator() overload is a constexpr function that forwards the operand
         * to the stored functor and invokes it. The invocation is performed by
         * using std::invoke to ensure compatibility with callable objects and member functions.
         *
         * @tparam T The type of the operand being passed to the functor.
         * @param Operand The argument to pass to the functor. It is perfectly
         *                forwarded to ensure the correct value category is preserved.
         * @return The result of invoking the functor with the provided operand.
         *         The deduced return type is determined by the functor's invocation result.
         *
         * @note This overload is restricted to be called on an lvalue.
         */
        template <typename T>
            requires std::invocable<F &, T>
        constexpr decltype(auto) operator()(T &&Operand) & {
            return std::invoke(Functor, std::forward<T>(Operand));
        }

        /**
         * Applies the stored functor to the given operand parameter.
         *
         * This operator() overload is a constexpr function that forwards the operand
         * to the stored functor and invokes it. The invocation is performed by
         * using std::invoke to ensure compatibility with callable objects and member functions.
         *
         * @tparam T The type of the operand being passed to the functor.
         * @param Operand The argument to pass to the functor. It is perfectly
         *                forwarded to ensure the correct value category is preserved.
         * @return The result of invoking the functor with the provided operand.
         *         The deduced return type is determined by the functor's invocation result.
         *
         * @note This overload is restricted to be called on an lvalue.
         */
        template <typename T>
            requires std::invocable<const F &, T>
        constexpr decltype(auto) operator()(T &&Operand) const & {
            return std::invoke(Functor, std::forward<T>(Operand));
        }

        /**
         * Applies the stored functor to the given operand parameter.
         *
         * This operator() overload is a constexpr function that forwards the operand
         * to the stored functor and invokes it. The invocation is performed by
         * using std::invoke to ensure compatibility with callable objects and member functions.
         *
         * @tparam T The type of the operand being passed to the functor.
         * @param Operand The argument to pass to the functor. It is perfectly
         *                forwarded to ensure the correct value category is preserved.
         * @return The result of invoking the functor with the provided operand.
         *         The deduced return type is determined by the functor's invocation result.
         *
         * @note This overload is restricted to be called on an lvalue.
         */
        template <typename T>
            requires std::invocable<F, T>
        constexpr decltype(auto) operator()(T &&Operand) && {
            return std::invoke(std::move(Functor), std::forward<T>(Operand));
        }

        /**
         * An overloaded pipe operator that allows applying an ExtensionMethodClosure to a given operand.
         *
         * This operator enables a syntax where an operand can be "piped" through an extension method closure to
         * generate a result.
         *
         * @param Operand The input operand to which the extension method closure is applied. It is a forwarding
         * reference.
         * @param Closure The extension method closure to be applied to the operand.
         * @return The result of applying the extension method closure to the given operand.
         */
        template <typename T>
            requires std::invocable<F &, T>
        friend constexpr decltype(auto) operator|(T &&Operand, ExtensionMethodClosure &Closure) {
            return Closure(std::forward<T>(Operand));
        }

        /**
         * An overloaded pipe operator that allows applying an ExtensionMethodClosure to a given operand.
         *
         * This operator enables a syntax where an operand can be "piped" through an extension method closure to
         * generate a result.
         *
         * @param Operand The input operand to which the extension method closure is applied. It is a forwarding
         * reference.
         * @param Closure The extension method closure to be applied to the operand.
         * @return The result of applying the extension method closure to the given operand.
         */
        template <typename T>
            requires std::invocable<const F &, T>
        friend constexpr decltype(auto) operator|(T &&Operand, const ExtensionMethodClosure &Closure) {
            return Closure(std::forward<T>(Operand));
        }

        /**
         * An overloaded pipe operator that allows applying an ExtensionMethodClosure to a given operand.
         *
         * This operator enables a syntax where an operand can be "piped" through an extension method closure to
         * generate a result.
         *
         * @param Operand The input operand to which the extension method closure is applied. It is a forwarding
         * reference.
         * @param Closure The extension method closure to be applied to the operand.
         * @return The result of applying the extension method closure to the given operand.
         */
        template <typename T>
            requires std::invocable<F, T>
        friend constexpr decltype(auto) operator|(T &&Operand, ExtensionMethodClosure &&Closure) {
            return std::move(Closure)(std::forward<T>(Operand));
        }

      private:
        F Functor;
    };

    /**
     * ExtensionMethodConstClosure is a template structure that facilitates the use of
     * extension methods with a constant closure. It enables a functor to be used as an
     * extension method through operator overloading.
     *
     * The structure requires a valid functor object, which is validated through a custom
     * IsValidFunctorObject constraint. This ensures the passed functor is callable.
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
        requires(IsValidFunctorObject(Functor))
    struct ExtensionMethodConstClosure {

        /**
         * @brief Invokes the stored functor with the given operand.
         *
         * This operator utilizes std::invoke to call the stored Functor object
         * with the provided operand. It ensures that the operand is perfectly
         * forwarded to the invoked functor.
         *
         * @param Operand The argument to be forwarded to the Functor. It can be
         *        an lvalue or rvalue of any type that is compatible with Functor.
         * @return The result of invoking the Functor with the forwarded operand.
         *         The return type depends on the Functor and the operand type.
         */
        template <typename T>
            requires std::invocable<decltype(Functor), T>
        constexpr decltype(auto) operator()(T &&Operand) const {
            return std::invoke(Functor, std::forward<T>(Operand));
        }

        /**
         * @brief Overloads the pipe operator (|) to apply an extension method closure
         *        to a given operand.
         *
         * @tparam T The type of the operand being passed.
         * @param Operand An argument of type T, forwarded to the closure.
         * @param Closure A constant reference to an ExtensionMethodConstClosure that
         *        represents the extension method to be applied to the operand.
         * @return The result of invoking the closure with the provided operand, returned
         *         as an automatically deduced type.
         *
         * @note This operator is designed to enable a cleaner, functional-style syntax
         *       when applying extension methods to objects or values.
         *
         * @remark The constexpr specifier ensures this operator can be used in
         *         compile-time expressions if all involved components support it.
         */
        template <typename T>
            requires std::invocable<decltype(Functor), T>
        friend constexpr decltype(auto) operator|(T &&Operand, const ExtensionMethodConstClosure &Closure) {
            return Closure(std::forward<T>(Operand));
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
     * @requires Functor type must pass the `IsValidFunctorObject` check, ensuring
     *           it has a function call operator and, if a pointer, it should not be null.
     *
     * @note This struct operates on a constexpr basis, allowing its functionality
     *       to be invoked during compile time if all arguments are compile-time constants.
     */
    template <auto Functor>
        requires(IsValidFunctorObject(Functor))
    struct ExtensionMethodBinder {

        /**
         * Overloaded call operator that invokes the Functor with the provided arguments.
         *
         * @param Args The arguments to be forwarded to the Functor for invocation.
         * @return The result of invoking the Functor with the forwarded arguments.
         */
        template <typename... T>
            requires std::invocable<decltype(Functor), T...>
        constexpr auto operator()(T &&...Args) const {
            return std::invoke(Functor, std::forward<T>(Args)...);
        }

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
         * @param Args Forwarding reference to the arguments to be bound with the functor.
         *             These are perfect-forwarded, preserving their value category.
         *
         * @return A callable closure with the provided arguments bound to the functor.
         *         This closure can be utilized to execute the functor at a later point.
         */
        template <typename... T>
            requires(!std::invocable<decltype(Functor), T...>)
        constexpr auto operator()(T &&...Args) const {
            if constexpr (sizeof...(T) == 0) {
                return ExtensionMethodConstClosure<Functor>();
            } else {
                using BindingType = decltype(Retro::BindBack<Functor>(std::forward<T>(Args)...));
                return ExtensionMethodClosure<BindingType>(Retro::BindBack<Functor>(std::forward<T>(Args)...));
            }
        }
    };

    RETROLIB_EXPORT template <auto Functor>
        requires(IsValidFunctorObject(Functor))
    constexpr ExtensionMethodBinder<Functor> ExtensionMethod;
} // namespace retro