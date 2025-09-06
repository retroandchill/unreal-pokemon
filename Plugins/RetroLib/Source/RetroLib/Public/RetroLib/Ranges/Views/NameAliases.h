/**
 * @file NameAliases.h
 * @brief View name aliases for the STL
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Ranges/Concepts/Containers.h"

#if !RETROLIB_WITH_MODULES
#include <ranges>
#include <span>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro
{
    RETROLIB_EXPORT template <typename T, size_t E = std::dynamic_extent>
    using TSpan = std::span<T, E>;

    namespace Ranges::Views
    {
        RETROLIB_EXPORT template <typename T>
        constexpr auto Empty = std::ranges::views::empty<T>;

        RETROLIB_EXPORT constexpr auto Filter = std::ranges::views::filter;

        RETROLIB_EXPORT constexpr auto Transform = std::ranges::views::transform;

        RETROLIB_EXPORT constexpr auto Single = std::ranges::views::single;

        RETROLIB_EXPORT constexpr auto Iota = std::ranges::views::iota;

        RETROLIB_EXPORT template <typename T>
        constexpr auto IStream = std::ranges::views::istream<T>;

        RETROLIB_EXPORT constexpr auto All = std::ranges::views::all;

        RETROLIB_EXPORT constexpr auto Take = std::ranges::views::take;

        RETROLIB_EXPORT constexpr auto TakeWhile = std::ranges::views::take_while;

        RETROLIB_EXPORT constexpr auto Drop = std::ranges::views::drop;

        RETROLIB_EXPORT constexpr auto DropWhile = std::ranges::views::drop_while;

        RETROLIB_EXPORT constexpr auto Join = std::ranges::views::join;

        RETROLIB_EXPORT constexpr auto Split = std::ranges::views::split;

        RETROLIB_EXPORT constexpr auto LazySplit = std::ranges::views::lazy_split;

        RETROLIB_EXPORT constexpr auto Counted = std::ranges::views::counted;

        RETROLIB_EXPORT constexpr auto Common = std::ranges::views::common;

        RETROLIB_EXPORT constexpr auto Reverse = std::ranges::views::reverse;
    } // namespace Ranges::Views
} // namespace Retro