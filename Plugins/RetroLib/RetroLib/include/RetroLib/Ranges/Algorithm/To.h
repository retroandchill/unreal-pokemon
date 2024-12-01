/**
 * @file To.h
 * @brief Creates the view extension to convert it into a collection type.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "Ranges/Concepts/Containers.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {
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
        ContainerCompatibleRange<R, RangeCommonReference<R>> && std::constructible_from<C, A...>;

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
     * @param range The input range of elements to be transformed into the container.
     * @param args Additional arguments for constructing the container of type C.
     * @return A container of type C with elements from the input range appended to it.
     */
    RETROLIB_EXPORT template <typename C, std::ranges::input_range R, typename... A>
        requires(!std::ranges::view<C>) && CompatibleContainerTypeForArgs<C, R, A...>
    constexpr C to(R &&range, A &&...args) {
        C result(std::forward<A>(args)...);

        if constexpr (std::ranges::sized_range<R> && ReservableContainer<C>) {
            // We want to guarantee that we won't have any weird overflow issues when inserting into a container with
            // a mismatch between signed and unsigned sizes.
            assert(std::ranges::size(range) <= container_max_size(result));
            container_reserve(result, std::ranges::size(range));
        }

        using RangeType = RangeCommonReference<R>;
        for (auto &&x : range) {
            append_container(result, std::forward<RangeType>(x));
        }

        return result;
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
     * @param range The range to be converted.
     * @param args Additional arguments to be forwarded to the conversion operation.
     * @return An instance of the target container type containing the elements of the input range.
     */
    RETROLIB_EXPORT template <template <typename...> typename C, std::ranges::input_range R, typename... A>
    constexpr auto to(R &&range, A &&...args) {
        using RangeType = std::ranges::range_value_t<R>;
        return to<C<RangeType>>(std::forward<R>(range), std::forward<A>(args)...);
    }

    /**
     * @struct ToInvoker
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
    struct ToInvoker {
        /**
         * @brief Functor operator that converts a given range to a specified container type.
         *
         * This operator allows the conversion of a range into a specific container type by utilizing
         * the `to` function. It forwards the provided range and additional arguments to the `to` function.
         *
         * @tparam R The type of the range.
         * @tparam A Additional argument types.
         * @param range The range to be converted.
         * @param args Additional arguments to be forwarded to the `to` function.
         * @return A container of type C containing elements from the provided range.
         */
        template <std::ranges::input_range R, typename... A>
            requires CompatibleContainerTypeForArgs<C, R, A...>
        constexpr C operator()(R &&range, A &&...args) const {
            return to<C>(std::forward<R>(range), std::forward<A>(args)...);
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
    constexpr ToInvoker<C> to_invoker;

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
    struct TemplatedToInvoker {
        /**
         * Functor operator that applies a transformation to a given range.
         *
         * This operator utilizes the `to` function to convert or transform the given
         * range into a specified container type `C`. It forwards the range and any
         * additional arguments provided to the `to` function.
         *
         * @tparam R The type of the range.
         * @tparam A Additional argument types.
         * @param range The range of elements to be transformed or converted. It is
         *        forwarded as an rvalue reference to support different kinds of range
         *        inputs.
         * @param args Additional arguments that may be required by the transformation
         *        function `to`. These are also forwarded as rvalue references.
         *
         * @return The result of the transformation, encapsulated within a container of
         *         type `C`.
         */
        template <std::ranges::input_range R, typename... A>
        constexpr auto operator()(R &&range, A &&...args) const {
            return to<C>(std::forward<R>(range), std::forward<A>(args)...);
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
    constexpr TemplatedToInvoker<C> template_to_invoker;

    /**
     * Transforms the input arguments by applying the 'to' extension method with a specified invoker.
     *
     * @tparam C Template parameter representing the invoker type for the 'to' method.
     * @tparam A Variadic template parameter pack representing the types of the input arguments.
     *
     * @param args The input arguments to be transformed.
     *
     * @return An instance of the extension method with the specified invoker and input arguments.
     */
    RETROLIB_EXPORT template <typename C, typename... A>
        requires(!std::ranges::view<C>) && std::constructible_from<C, A...>
    constexpr auto to(A &&...args) {
        return extension_method<to_invoker<C>>(std::forward<A>(args)...);
    }

    /**
     * Converts the given arguments into a specific format or type by utilizing
     * an extension method with the specified template converter.
     *
     * @tparam C Template parameter representing the invoker type for the 'to' method.
     * @tparam A Variadic template parameter pack representing the types of the input arguments.
     *
     * @param args Variadic template arguments to be converted using the
     *             extension method.
     * @return The result of invoking the extension method with the specified
     *         converter and forwarded arguments, enabling conversion to the
     *         desired format or type.
     */
    RETROLIB_EXPORT template <template <typename...> typename C, typename... A>
    constexpr auto to(A &&...args) {
        return extension_method<template_to_invoker<C>>(std::forward<A>(args)...);
    }

} // namespace retro::ranges
