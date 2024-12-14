/**
 * @file FunctionTraits.h
 * @brief Contains the basic trait information for functional types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "TypeTraits.h"

#include <concepts>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * A traits structure to determine if a given type has exactly one function call operator.
     *
     * This template specialization inherits from `std::false_type` by default,
     * indicating that the specified type does not have a single function call operator.
     *
     * To extend the functionality for a specific type that has exactly one
     * function call operator, you need to provide a template specialization
     * for that type which inherits from `std::true_type`.
     *
     * Template Parameter:
     * - typename: The type to be checked for a single function call operator.
     */
    template <typename>
    struct HasOneFunctionCallOperator : std::false_type {};

    /**
     * @brief Determines if a class has exactly one function call operator.
     *
     * This template struct inherits from std::true_type if the specified
     * type T has exactly one function call operator. It makes use of
     * SFINAE (Substitution Failure Is Not An Error) principle by requiring
     * that T has a member function pointer to operator().
     *
     * @tparam T The class type to be inspected for a function call operator.
     *
     * The check is valid at compile time and is useful in template
     * metaprogramming where knowing the presence of a callable operator
     * can conditionally enable or disable certain code paths.
     */
    template <typename T>
        requires std::is_member_function_pointer_v<decltype(&T::operator())>
    struct HasOneFunctionCallOperator<T> : std::true_type {};

    /**
     * @class WithOp
     *
     * @brief Represents a callable object that can be executed using the function call operator.
     *
     * The WithOp struct provides an override of the function call operator, enabling instances
     * of this struct to be used as callable objects. This is used for utility checking to see if a type has
     * a valid function call operator or not.
     */
    struct WithOp {
        void operator()() const;
    };

    /**
     * @class Mixin
     *
     * @brief A template structure that combines the functionality of two types.
     *
     * The Mixin struct is a template that inherits from the specified type `T` and the
     * `WithOp` struct. This allows the Mixin to extend the capabilities of `T` by
     * adding the callable operator functionality provided by `WithOp`.
     * Ideal for scenarios where a type needs to exhibit both its original
     * behavior and additional operations defined in `WithOp`.
     *
     * This is used for a type trait check to see if a type has a functional callback at all, by checking if the
     * `operator()` overload can be resolved to a function pointer.
     *
     * @tparam T The type to be extended with additional callable operations.
     */
    template <typename T>
    struct Mixin : T, WithOp {};

    /**
     * @brief Concept to determine if a type does not have a single function call operator.
     *
     * This concept evaluates to true if the specified type T, when mixed with a certain `Mixin`,
     * does not possess exactly one function call operator. It leverages a helper named
     * `HasOneFunctionCallOperator` for this evaluation.
     *
     * @tparam T The type to be checked for the function call operator presence.
     */
    RETROLIB_EXPORT template <typename T>
    concept HasFunctionCallOperator =
        std::is_pointer_v<T> || std::is_member_pointer_v<T> || !HasOneFunctionCallOperator<Mixin<T>>::value;

    /**
     * @brief Checks if a given object is a valid functor.
     *
     * This function evaluates whether an object can be considered a valid functor.
     * It uses compile-time checks to infer if the object has a function call operator.
     * If the object is a pointer or a member pointer, it also verifies that the pointer is not null.
     *
     * @tparam F The type of the functor object to be checked.
     * @param functor The instance of the functor object to be validated.
     * @return True if the object is a valid functor, otherwise false.
     */
    RETROLIB_EXPORT template <typename F>
    consteval bool is_valid_functor_object(const F &functor) {
        if constexpr (!HasFunctionCallOperator<F>) {
            return false;
        } else if constexpr (std::is_pointer_v<F> || std::is_member_pointer_v<F>) {
            return functor != nullptr;
        } else {
            return true;
        }
    }

    /**
     * FunctionTraitsBase is a template structure that provides introspection
     * capabilities for function-like types, including return type and argument types.
     *
     * @tparam R The return type of the function.
     * @tparam Free Indicates if this is a free function.
     * @tparam NoExcept A boolean value representing whether the member function is noexcept.
     * @tparam A The argument types of the function.
     *
     * This structure derives from ValidType to indicate it represents a valid function type.
     */
    template <typename R, bool Free, bool NoExcept, typename... A>
    struct FunctionTraitsBase : ValidType {
        /**
         * Indicates if this is a free function.
         */
        static constexpr bool is_free = Free;

        /**
         * Identifies the return type of the function.
         */
        using ReturnType = R;

        /**
         * The number of arguments the function takes.
         */
        static constexpr size_t arg_count = sizeof...(A);

        /**
         * Identifies the type of the N-th argument of the function.
         *
         * @tparam The argument to get
         * @requires `N` must be less than `arg_count`.
         */
        template <size_t N>
            requires(N < arg_count)
        using Arg = std::tuple_element_t<N, std::tuple<A...>>;

        /**
         * A boolean value representing whether the member function is noexcept.
         */
        static constexpr bool is_noexcept = NoExcept;
    };

    /**
     * @enum RefQualifier
     * @brief Represents the reference qualifiers in C++ for distinguishing between
     *        lvalue references, rvalue references, and no reference.
     *
     * RefQualifier is used to identify the type of reference qualifier that applies
     * to a particular entity in C++. This is particularly useful in template
     * metaprogramming and function overloading where distinguishing between
     * reference types is necessary.
     */
    enum class RefQualifier {
        /**
         * Indicates no reference qualifier.
         */
        None,

        /**
         * Indicates an lvalue reference qualifier (&).
         */
        LValue,

        /**
         * Indicates an rvalue reference qualifier (&&).
         */
        RValue
    };

    /**
     * @brief MethodTraitsBase provides type traits for member functions.
     *
     * This struct extends the capabilities of FunctionTraitsBase to include
     * traits specific to member functions of user-defined types. It encapsulates
     * information about the const-qualification, reference qualification, and
     * noexcept specification of member functions.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam Const A boolean value representing whether the member function is const-qualified.
     * @tparam Ref The reference qualifier (lvalue, rvalue, or none) of the member function.
     * @tparam NoExcept A boolean value representing whether the member function is noexcept.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, bool Const, RefQualifier Ref, bool NoExcept, typename... A>
    struct MethodTraitsBase : FunctionTraitsBase<R, false, NoExcept, A...> {
        /**
         * The class type to which the member function belongs.
         */
        using ClassType = C;

        /**
         * A boolean value representing whether the member function is const-qualified.
         */
        static constexpr bool is_const = Const;

        /**
         * The reference qualifier (lvalue, rvalue, or none) of the member function.
         */
        static constexpr RefQualifier ref_qualifier = Ref;
    };

    /**
     * A template structure used to define function traits for a given type.
     *
     * This class template is used primarily as a base case for when the provided
     * type does not meet the criteria for a valid function, thus it inherits
     * from `InvalidType`.
     *
     * The template is specialized further to extract various function traits
     * such as return type, arguments, arity, etc., for valid function types.
     * If the type does not correspond to a function, it defaults to inheriting
     * the properties of `InvalidType`, which indicates invalidity through
     * its static member `is_valid`.
     *
     * @tparam Unspecified type for which function traits are to be determined.
     */
    template <typename>
    struct FunctionTraits : InvalidType {};

    /**
     * FunctionTraits is a template structure that specializes the FunctionTraitsBase
     * for function pointers, allowing introspection of return types and argument
     * types specifically for this kind of callable entity.
     *
     * @tparam R The return type of the function pointer.
     * @tparam A The argument types of the function pointer.
     *
     * This specialization allows using FunctionTraitsBase capabilities specifically
     * for function pointers, enabling developers to extract function properties
     * like return type and the number of arguments from a function pointer type.
     */
    template <typename R, typename... A>
    struct FunctionTraits<R (*)(A...)> : FunctionTraitsBase<R, true, false, A...> {};

    /**
     * FunctionTraits is a template specialization of FunctionTraitsBase used to
     * derive traits for free functions that are marked with noexcept.
     *
     * This specialization provides compile-time introspection capabilities for
     * such function pointers by inheriting from FunctionTraitsBase. These capabilities
     * include identifying the return type, argument types, and properties such as
     * whether the function is noexcept.
     *
     * @tparam R The return type of the function.
     * @tparam A The types of the arguments accepted by the function.
     */
    template <typename R, typename... A>
    struct FunctionTraits<R (*)(A...) noexcept> : FunctionTraitsBase<R, true, true, A...> {};

    /**
     * @brief FunctionTraits provides type traits for non-const member functions.
     *
     * This specialization of FunctionTraits is designed for member functions that are
     * neither const-qualified nor have reference qualifiers. It derives from
     * MethodTraitsBase, inheriting capabilities specific to member functions.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The types of arguments the member function takes.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...)> : MethodTraitsBase<C, R, false, RefQualifier::None, false, A...> {};

    /**
     * @brief FunctionTraits specialization for const-qualified member function pointers.
     *
     * This struct provides type traits for member functions that are const-qualified.
     * It extends MethodTraitsBase to encapsulate details specific to this category
     * of member functions. This includes providing type information about the class
     * to which the member function belongs, the return type, and the argument types.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const> : MethodTraitsBase<C, R, true, RefQualifier::None, false, A...> {};

    /**
     * @brief Provides type traits for member functions with lvalue reference qualifier.
     *
     * This specialization of the FunctionTraits struct is used to deduce and encapsulate
     * traits of non-const member functions of classes that are qualified with an lvalue reference (&).
     * It inherits from MethodTraitsBase, providing detailed information about the function,
     * including its class type, return type, and argument types, as well as indicating
     * that the function has an lvalue reference qualifier.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) &> : MethodTraitsBase<C, R, false, RefQualifier::LValue, false, A...> {};

    /**
     * @brief Provides function traits for const-qualified member functions with lvalue reference qualifier.
     *
     * This template specialization of FunctionTraits extracts type information
     * from member functions that are const-qualified and have an lvalue reference qualifier.
     * It uses the MethodTraitsBase class to encapsulate this information and extend the
     * FunctionTraitsBase capabilities specifically for member function pointers.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const &> : MethodTraitsBase<C, R, true, RefQualifier::LValue, false, A...> {};

    /**
     * @brief Provides type traits for rvalue reference qualified member functions.
     *
     * This specialization of FunctionTraits is used to determine and provide
     * compile-time information about rvalue reference qualified member functions
     * (functions that use the && qualifier). It inherits from MethodTraitsBase,
     * providing detailed traits about the member function, such as its class type,
     * return type, and argument types.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) &&> : MethodTraitsBase<C, R, false, RefQualifier::RValue, false, A...> {};

    /**
     * @brief FunctionTraits specialization for rvalue reference qualified const member functions.
     *
     * This struct is a specialization of FunctionTraits for handling member functions
     * that are rvalue reference qualified and const-qualified. It provides type traits
     * specific to such member functions, enabling inspection of their return type,
     * argument types, and qualification properties.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const &&> : MethodTraitsBase<C, R, true, RefQualifier::RValue, false, A...> {};

    /**
     * @brief Provides type traits for noexcept member functions.
     *
     * FunctionTraits is a specialization for member functions that are marked as noexcept.
     * It derives from MethodTraitsBase and provides detailed information about the
     * member function's class type, return type, argument types, and modifies the
     * base behavior to note that the member function is noexcept and has no reference
     * qualifier.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) noexcept> : MethodTraitsBase<C, R, false, RefQualifier::None, true, A...> {};

    /**
     * @brief Provides type traits for const noexcept member functions.
     *
     * FunctionTraits is a specialized struct for analyzing const-qualified, noexcept
     * member functions of a class type. It extends MethodTraitsBase to inherit
     * basic member function trait capabilities.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const noexcept>
        : MethodTraitsBase<C, R, true, RefQualifier::None, true, A...> {};

    /**
     * @brief FunctionTraits specialization for noexcept member functions with lvalue reference qualifier.
     *
     * This class template is a specialization of the FunctionTraits structure, providing type traits specifically
     * for member functions of a class that are qualified with an lvalue reference (&) and specified as noexcept.
     * It derives from MethodTraitsBase, inheriting traits and extending them for member functions that fit these
     * criteria.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) & noexcept> : MethodTraitsBase<C, R, false, RefQualifier::LValue, true, A...> {
    };

    /**
     * @brief Provides type traits for const-qualified, lvalue-referenced, noexcept member functions.
     *
     * This specialization of FunctionTraits is designed to extract and provide type information
     * for member functions that are const-qualified, lvalue-referenced, and marked as noexcept.
     * It inherits from MethodTraitsBase, which offers utilities for accessing characteristics
     * of such member functions, including the class type, return type, argument types,
     * and qualifiers.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The types of the arguments that the member function takes.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const & noexcept>
        : MethodTraitsBase<C, R, true, RefQualifier::LValue, true, A...> {};

    /**
     * @brief FunctionTraits provides type traits for a specific category of member functions.
     *
     * This specialization of FunctionTraits defines traits for member functions that are
     * qualified as rvalue references and are marked as noexcept. It serves as a type trait
     * utility for extracting information about the member function's class, return type,
     * argument types, and other qualifiers such as rvalue reference and noexcept.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) && noexcept>
        : MethodTraitsBase<C, R, false, RefQualifier::RValue, true, A...> {};

    /**
     * @brief Specialization of FunctionTraits for rvalue reference, const, noexcept member functions.
     *
     * This structure provides type traits for const member functions with an rvalue reference qualifier
     * and a noexcept specification. It extends MethodTraitsBase to include specific type traits for
     * such member functions of user-defined types.
     *
     * @tparam C The class type to which the member function belongs.
     * @tparam R The return type of the member function.
     * @tparam A The argument types taken by the member function.
     */
    template <typename C, typename R, typename... A>
    struct FunctionTraits<R (C::*)(A...) const && noexcept>
        : MethodTraitsBase<C, R, true, RefQualifier::RValue, true, A...> {};

    /**
     * Specialization of the FunctionTraits structure for types with a single
     * function call operator.
     *
     * This specialization enables the extraction of function traits for objects
     * that define a single function call operator. It relies on the `HasOneFunctionCallOperator`
     * concept to ensure that the given type `T` has exactly one function call operator.
     * The traits of the type `T` are then derived from the traits of the function call
     * operator through the use of `decltype(&T::operator())`.
     *
     * @tparam T A type that must have a single function call operator.
     */
    template <typename T>
        requires HasOneFunctionCallOperator<T>::value
    struct FunctionTraits<T> : FunctionTraits<decltype(&T::operator())> {
        static constexpr bool is_free = true;
    };

    /**
     * @class MemberTraits
     *
     * @brief A struct used to derive type traits indicating the presence or absence of specific member features.
     *
     * The MemberTraits struct serves as a utility to enhance type introspection by inheriting from InvalidType.
     * It is typically used in template metaprogramming to determine and enforce type characteristics and constraints.
     */
    template <typename>
    struct MemberTraits : InvalidType {};

    /**
     * @struct MemberTraits
     *
     * @brief Provides type information about member pointers.
     *
     * The MemberTraits struct template is used to extract type information from member pointers.
     * It inherits from ValidType and defines two type aliases: ClassType and MemberType.
     * ClassType corresponds to the class type C to which the member belongs, and MemberType corresponds to the type M
     * of the member itself.
     *
     * This is useful in template metaprogramming where type traits and introspection of member pointers are required.
     *
     * @tparam M Type of the member.
     * @tparam C Type of the class containing the member.
     */
    template <typename C, typename M>
    struct MemberTraits<M C::*> : ValidType {
        using ClassType = C;
        using MemberType = M;
    };

    /**
     * @brief Concept to determine if a type is a functional type.
     *
     * This concept checks whether a given type T satisfies the criteria for being considered
     * a functional type. A type is considered a functional type if it meets one of the following
     * conditions:
     *
     * - The type has valid function traits as determined by the FunctionTraits<T>::is_valid.
     * - The type possesses a function call operator, as indicated by the HasFunctionCallOperator<T>.
     *
     * This concept is useful for template metaprogramming scenarios where determining if a type
     * can be called as a function is necessary.
     *
     * @tparam T The type to be checked against the functional-type criteria.
     */
    RETROLIB_EXPORT template <typename T>
    concept FunctionalType = FunctionTraits<T>::is_valid;

    /**
     * @brief Concept to determine if a type T is a free function.
     *
     * This concept checks if the type T satisfies two conditions:
     * 1. T must satisfy the FunctionalType concept, meaning it must be a type
     *    that can be used as a function or callable object.
     * 2. The traits of the type T, as defined by FunctionTraits<T>, must indicate
     *    that it is a free function. Specifically, FunctionTraits<T>::is_free
     *    must be true.
     *
     * @tparam T The type to be evaluated as a free function.
     */
    RETROLIB_EXPORT template <typename T>
    concept FreeFunction = FunctionalType<T> && FunctionTraits<T>::is_free;

    /**
     * @brief Concept to determine if a type is a method.
     *
     * This concept checks if a given type `T` is a method. A type is considered
     * a method if it satisfies two conditions:
     * - It is a functional type, as determined by the `FunctionalType<T>` concept.
     * - It is not a free function, as indicated by the `FunctionTraits<T>::is_free`.
     *
     * @tparam T The type to check against the concept.
     */
    RETROLIB_EXPORT template <typename T>
    concept Method = FunctionalType<T> && (!FunctionTraits<T>::is_free);

    /**
     * The return type of the given functional type.
     *
     * @tparam T The functional type
     */
    RETROLIB_EXPORT template <FunctionalType T>
    using ReturnType = typename FunctionTraits<T>::ReturnType;

    /**
     * The argument count of the given functional type.
     *
     * @tparam T The functional type
     */
    RETROLIB_EXPORT template <FunctionalType T>
    constexpr size_t argument_count = FunctionTraits<T>::arg_count;

    /**
     * The N-th argument count of the given functional type.
     *
     * @tparam T The functional type
     * @tparam N The index of the argument
     */
    RETROLIB_EXPORT template <FunctionalType T, size_t N>
        requires(N < argument_count<T>)
    using Argument = typename FunctionTraits<T>::template Arg<N>;

    /**
     * Concept for checking if a type is a class member.
     *
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept Member = MemberTraits<T>::is_valid || Method<T>;

    /**
     * The owning class of the given method.
     *
     * @tparam T The method type
     */
    RETROLIB_EXPORT template <Method T>
    using FunctionClassType = typename FunctionTraits<T>::ClassType;

    /**
     * The owning class of the given data members.
     *
     * @tparam T The method type
     */
    RETROLIB_EXPORT template <Member T>
    using MemberClassType = typename MemberTraits<T>::ClassType;

    /**
     * @brief Concept to check for const-qualified types.
     *
     * The ConstQualified concept ensures that a given type satisfies two conditions:
     * 1. It meets the Method<T> concept.
     * 2. It has a static member `is_const` that evaluates to true.
     *
     * This concept can be used to constrain template parameters to types that
     * represent methods and are const-qualified.
     *
     * @tparam T The type to be checked for const qualification and conformity to Method<T>.
     */
    RETROLIB_EXPORT template <typename T>
    concept ConstQualified = Method<T> && FunctionTraits<T>::is_const;

    /**
     * @brief Concept to check if a type T has a method and is lvalue-qualified.
     *
     * This concept ensures that a given type T:
     * - Satisfies the Method<T> requirement.
     * - Has a reference qualifier equal to RefQualifier::LValue, indicating that the method
     *   is qualified to be called on lvalues.
     *
     * @tparam T The type to be checked against the concept requirements.
     */
    RETROLIB_EXPORT template <typename T>
    concept LValueQualified = Method<T> && (FunctionTraits<T>::ref_qualifier == RefQualifier::LValue);

    /**
     * Concept to determine if a given type `T` is qualified with an rvalue reference.
     *
     * This concept checks two conditions:
     * 1. `T` must satisfy the `Method` concept.
     * 2. `T` must have an rvalue reference qualifier, indicated by `T::ref_qualifier` being equal to
     * `RefQualifier::RValue`.
     *
     * @tparam T The type to be checked against the concept.
     */
    RETROLIB_EXPORT template <typename T>
    concept RValueQualified = Method<T> && (FunctionTraits<T>::ref_qualifier == RefQualifier::RValue);

    /**
     * Get the type of the owning class of a method, adding const to the type, if the method is const-qualified.
     *
     * @tparam T The method type in question
     */
    RETROLIB_EXPORT template <Method T>
    using ConstQualifiedClassType =
        std::conditional_t<ConstQualified<T>, const FunctionClassType<T>, FunctionClassType<T>>;

    /**
     * Get the reference type of the owning class of a method, adding const to the type, if the method is
     * const-qualified.
     *
     * @tparam T The method type in question
     */
    RETROLIB_EXPORT template <Method T>
    using RefQualifiedClassType =
        std::conditional_t<RValueQualified<T>, ConstQualifiedClassType<T> &&, ConstQualifiedClassType<T> &>;

    /**
     * A concept that checks if a given type T satisfies two conditions:
     * 1. The type T must satisfy the Method concept. This implies T must meet
     *    specific requirements defined by Method.
     * 2. The type T must have a static member constant or method named is_noexcept,
     *    which should evaluate to true.
     *
     * This concept is useful for ensuring that certain operations or methods
     * associated with the type T can be called without throwing exceptions.
     *
     * Template Parameter:
     * - T: The type to check against the NoExcept concept.
     */
    RETROLIB_EXPORT template <typename T>
    concept NoExcept = Method<T> && FunctionTraits<T>::is_noexcept;

} // namespace retro