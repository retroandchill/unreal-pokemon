/**
 * @file To.h
 * @brief Creates the view extension to convert it into a collection type.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/Tuples.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/Concepts.h"
#include "RetroLib/Ranges/FeatureBridge.h"

#if !RETROLIB_WITH_MODULES
#include <map>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
    /**
     * @struct TIsMap
     * @brief A type trait structure that indicates whether a given type is a map.
     *
     * This struct is a specialization of `std::false_type` and represents a compile-time
     * boolean constant. It will evaluate to `false` for any type unless explicitly
     * specialized for map-like types.
     *
     * Use this structure to perform type-checking operations for map-like types
     * in template metaprogramming scenarios.
     */
    RETROLIB_EXPORT template <template <typename...> typename>
    struct TIsMap : std::false_type {};

    /**
     * @brief Trait to identify the std::map type.
     *
     * This specialization of the IsMap struct evaluates to std::true_type
     * for the std::map type, indicating that the given type is considered
     * a map-like container. It provides a convenient way to detect maps
     * in template metaprogramming.
     *
     * @tparam std::map The specialization is explicitly defined for std::map.
     *
     * The primary use of this struct is to help with type identification
     * and enable conditional compilation based on whether a type is a
     * map-like container.
     */
    RETROLIB_EXPORT template <>
    struct TIsMap<std::map> : std::true_type {};

    /**
     * @brief A utility struct that defines a template alias to create a type
     *        based on the range value type of a given range.
     *
     * The `FromRange` struct provides a mechanism to derive a type `C` using
     * the element type of a given range `R`. This is achieved through the
     * `Invoke` template alias, where `C` is parameterized with the value type
     * of the specified range `R`.
     *
     * @tparam R The input range type that satisfies the C++ `std::ranges`
     *        concept. The type must provide a valid range value type,
     *        accessible via `std::ranges::range_value_t`.
     */
    RETROLIB_EXPORT template <template <typename...> typename C>
    struct TFromRange {
        template <typename R>
        using Invoke = C<std::ranges::range_value_t<R>>;
    };

    /**
     * @brief A template meta-structure for deriving a container type based on a range with tuple-like elements.
     *
     * @tparam C The container to be modified or constructed.
     *
     * This structure operates on a given range type `R` and ensures that the range's value type is tuple-like,
     * with a tuple size of exactly 2. It then extracts the types of the tuple's first and second elements
     * and uses them as the template parameters for the container `C`.
     *
     * Template Constraints:
     * - The range value type of `R` must satisfy the `TupleLike` concept.
     * - The size of the tuple-like value type must be exactly 2.
     *
     * Type Alias:
     * - `Invoke`: Alias for `C` instantiated with the first and second tuple element types, respectively.
     */
    RETROLIB_EXPORT template <template <typename...> typename C>
        requires TIsMap<C>::value
    struct TFromRange<C> {
        template <typename R>
            requires TupleLike<std::ranges::range_value_t<R>> && (std::tuple_size_v<std::ranges::range_value_t<R>> == 2)
        using Invoke = C<std::tuple_element_t<0, std::ranges::range_value_t<R>>,
                         std::tuple_element_t<1, std::ranges::range_value_t<R>>>;
        ;
    };

    /**
     * Concept used for doing checks on a compatible container type.
     *
     * @tparam C The type of container to construct
     * @tparam R The type of range to convert
     * @tparam A The constructor arguments for the container
     */
    template <typename C, typename R, typename... A>
    concept CompatibleContainerTypeForArgs =
        std::ranges::input_range<R> && AppendableContainer<C, std::ranges::range_value_t<R>> &&
        ContainerCompatibleRange<R, TRangeCommonReference<R>> && std::constructible_from<C, A...>;

    /**
     * Transforms the given range into a container of type C using provided arguments for construction.
     *
     * This function constructs a container of type C using the provided arguments. If the range is a sized
     * range and the container can be reserved, the function ensures that the container has sufficient capacity
     * to hold all elements in the range to prevent overflow issues. It then appends each element from the range
     * into the container.
     *
     * @tparam C The container type to create
     * @tparam R The type of the added range
     * @tparam A The additional arguments to use
     * @param Range The input range of elements to be transformed into the container.
     * @param Args Additional arguments for constructing the container of type C.
     * @return A container of type C with elements from the input range appended to it.
     */
    RETROLIB_EXPORT template <typename C, std::ranges::input_range R, typename... A>
        requires(!std::ranges::view<C>) && CompatibleContainerTypeForArgs<C, R, A...>
    constexpr C To(R &&Range, A &&...Args) {
        C Result(std::forward<A>(Args)...);

        if constexpr (std::ranges::sized_range<R> && ReservableContainer<C>) {
            // We want to guarantee that we won't have any weird overflow issues when inserting into a container with
            // a mismatch between signed and unsigned sizes.
            RETROLIB_ASSERT(std::ranges::size(Range) <= static_cast<std::ranges::range_size_t<R>>(ContainerMaxSize(Result)));
            ContainerReserve(Result, std::ranges::size(Range));
        }

        using RangeType = TRangeCommonReference<R>;
        for (auto &&x : Range) {
            AppendContainer(Result, std::forward<RangeType>(x));
        }

        return Result;
    }

    /**
     * @brief Converts a range to a specified container type.
     *
     * This function template is designed to convert a provided range into a
     * container of the type specified by the template parameter C. It forwards
     * the range and any additional arguments to the conversion function.
     *
     * @tparam C Template parameter deducing the target container type.
     * @tparam R The type of the input range.
     * @tparam A Variadic arguments to be forwarded.
     * @param Range The range to be converted.
     * @param Args Additional arguments to be forwarded to the conversion operation.
     * @return An instance of the target container type containing the elements of the input range.
     */
    RETROLIB_EXPORT template <template <typename...> typename C, std::ranges::input_range R, typename... A>
    constexpr auto To(R &&Range, A &&...Args) {
        return To<typename TFromRange<C>::template Invoke<R>>(std::forward<R>(Range), std::forward<A>(Args)...);
    }

    /**
     * @struct TToInvoker
     *
     * A functor-like structure that facilitates the invocation of the `to` function template.
     *
     * @tparam C The type to which the input range and additional arguments will be converted.
     *
     * @details This structure defines a single function call operator that takes a range and
     * a variable number of additional arguments, forwarding them to the `to` function for
     * conversion to the specified type C.
     */
    template <typename C>
        requires(!std::ranges::view<C>)
    struct TToInvoker {
        /**
         * @brief Functor operator that converts a given range to a specified container type.
         *
         * This operator allows the conversion of a range into a specific container type by utilizing
         * the `to` function. It forwards the provided range and additional arguments to the `to` function.
         *
         * @tparam R The type of the range.
         * @tparam A Additional argument types.
         * @param Range The range to be converted.
         * @param Args Additional arguments to be forwarded to the `to` function.
         * @return A container of type C containing elements from the provided range.
         */
        template <std::ranges::input_range R, typename... A>
            requires CompatibleContainerTypeForArgs<C, R, A...>
        constexpr C operator()(R &&Range, A &&...Args) const {
            return To<C>(std::forward<R>(Range), std::forward<A>(Args)...);
        }
    };

    /**
     * A compile-time constant instance of the ToInvoker class template.
     *
     * This object can be used to perform operations defined by the ToInvoker class template specific to the type C.
     *
     * @tparam C The type for which the ToInvoker operations will be defined.
     */
    template <typename C>
        requires(!std::ranges::view<C>)
    constexpr TToInvoker<C> ToCallback;

    /**
     * A struct providing a function call operator that invokes the `to` function template.
     *
     * This struct defines a templated function call operator that accepts an input range
     * and a variadic list of arguments, and forwards them to the `to` function template
     * specialized with the template parameter `C`.
     *
     * @tparam C The type to which the input range and additional arguments will be converted.
     */
    template <template <typename...> typename C>
    struct TTemplatedToInvoker {
        /**
         * Functor operator that applies a transformation to a given range.
         *
         * This operator utilizes the `to` function to convert or transform the given
         * range into a specified container type `C`. It forwards the range and any
         * additional arguments provided to the `to` function.
         *
         * @tparam R The type of the range.
         * @tparam A Additional argument types.
         * @param Range The range of elements to be transformed or converted. It is
         *        forwarded as an rvalue reference to support different kinds of range
         *        inputs.
         * @param Args Additional arguments that may be required by the transformation
         *        function `to`. These are also forwarded as rvalue references.
         *
         * @return The result of the transformation, encapsulated within a container of
         *         type `C`.
         */
        template <std::ranges::input_range R, typename... A>
        constexpr auto operator()(R &&Range, A &&...Args) const {
            return To<C>(std::forward<R>(Range), std::forward<A>(Args)...);
        }
    };

    /**
     * @brief A constant instance of TemplatedToInvoker with template parameter C.
     *
     * This variable acts as an invoker for templated functions associated with
     * the TemplatedToInvoker class template. It facilitates calling member
     * functions or performing operations defined by TemplatedToInvoker for
     * objects of type C.
     *
     * @tparam C The type for which the invoker is specialized.
     */
    template <template <typename...> typename C>
    constexpr TTemplatedToInvoker<C> TemplateToCallback;

    /**
     * Transforms the input arguments by applying the 'to' extension method with a specified invoker.
     *
     * @tparam C Template parameter representing the invoker type for the 'to' method.
     * @tparam A Variadic template parameter pack representing the types of the input arguments.
     *
     * @param Args The input arguments to be transformed.
     *
     * @return An instance of the extension method with the specified invoker and input arguments.
     */
    RETROLIB_EXPORT template <typename C, typename... A>
        requires(!std::ranges::view<C>) && std::constructible_from<C, A...>
    constexpr auto To(A &&...Args) {
        return ExtensionMethod<ToCallback<C>>(std::forward<A>(Args)...);
    }

    /**
     * Converts the given arguments into a specific format or type by utilizing
     * an extension method with the specified template converter.
     *
     * @tparam C Template parameter representing the invoker type for the 'to' method.
     * @tparam A Variadic template parameter pack representing the types of the input arguments.
     *
     * @param Args Variadic template arguments to be converted using the
     *             extension method.
     * @return The result of invoking the extension method with the specified
     *         converter and forwarded arguments, enabling conversion to the
     *         desired format or type.
     */
    RETROLIB_EXPORT template <template <typename...> typename C, typename... A>
    constexpr auto To(A &&...Args) {
        return ExtensionMethod<TemplateToCallback<C>>(std::forward<A>(Args)...);
    }

} // namespace retro::ranges
