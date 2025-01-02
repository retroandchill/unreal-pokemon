/**
 * @file NameAliases.h
 * @brief View name aliases for the STL
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include <ranges>
#endif

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Functional/FunctionalClosure.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    RETROLIB_EXPORT template <typename T>
    constexpr auto Empty = std::ranges::views::empty<T>;

    RETROLIB_EXPORT constexpr auto Single = std::ranges::views::single;

    RETROLIB_EXPORT constexpr auto Iota = std::ranges::views::iota;

    RETROLIB_EXPORT template <typename T>
    constexpr auto IStream = std::ranges::views::istream<T>;

    RETROLIB_EXPORT constexpr auto All = std::ranges::views::all;

    RETROLIB_EXPORT constexpr auto Take = std::ranges::views::take;

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::views::take_while, TakeWhile)

    RETROLIB_EXPORT constexpr auto Drop = std::ranges::views::drop;

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::views::drop_while, DropWhile)

    RETROLIB_EXPORT constexpr auto Join = std::ranges::views::join;

    RETROLIB_EXPORT constexpr auto Split = std::ranges::views::split;

    RETROLIB_EXPORT constexpr auto LazySplit = std::ranges::views::lazy_split;

    RETROLIB_EXPORT constexpr auto Counted = std::ranges::views::counted;

    RETROLIB_EXPORT constexpr auto Common = std::ranges::views::common;

    RETROLIB_EXPORT constexpr auto Reverse = std::ranges::views::reverse;

}