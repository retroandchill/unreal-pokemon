/**
 * @file ParameterPacks.h
 * @brief Concepts for working with parameter packs
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <tuple>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * Extract the decayed version of the Nth type from the parameter pack.
     *
     * @tparam N The index of the parameter pack
     * @tparam T The types of the parameter pack
     */
    RETROLIB_EXPORT template <size_t N, typename... T>
        requires(N < sizeof...(T))
    using PackType = std::tuple_element_t<N, std::tuple<std::decay_t<T>...>>;

    /**
     * @brief Concept to verify if a parameter pack contains exactly one type that is the same as a specified type.
     *
     * This concept checks if a provided parameter pack contains exactly one type and that this type is identical
     * to a specified type T. It is used to enforce compile-time constraints where a type pack is expected
     * to contain exactly one type matching a given criterion.
     *
     * @tparam T The specified type to compare against.
     * @tparam A The parameter pack to be checked.
     */
    RETROLIB_EXPORT template <typename T, typename... A>
    concept PackSameAs = (sizeof...(A) == 1) && std::same_as<PackType<0, A...>, T>;

    RETROLIB_EXPORT template <typename... T>
    using LastInPack = std::tuple_element_t<sizeof...(T) - 1, std::tuple<T...>>;

    template <typename T, typename... A>
    concept ConvertibleFromPack = (sizeof...(A) == 1) && std::convertible_to<PackType<0, A...>, T>;
} // namespace retro