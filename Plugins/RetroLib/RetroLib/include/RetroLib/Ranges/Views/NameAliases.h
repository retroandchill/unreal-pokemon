/**
 * @file NameAliases.h
 * @brief View name aliases for the STL
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    RETROLIB_EXPORT template <std::ranges::viewable_range R>
    using AllType = std::ranges::views::all_t<R>;

    RETROLIB_EXPORT constexpr auto All = std::ranges::views::all;

    RETROLIB_EXPORT constexpr auto Iota = std::ranges::views::iota;

    RETROLIB_EXPORT constexpr auto Join = std::ranges::views::join;

    RETROLIB_EXPORT constexpr auto Reverse = std::ranges::views::reverse;

    RETROLIB_EXPORT constexpr auto Take = std::ranges::views::take;

}