/**
 * @file NameAliases.h
 * @brief Aliases for STL range operations.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <algorithm>
#endif

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/Concepts/Containers.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    RETROLIB_EXPORT constexpr auto AllOf = ExtensionMethod<std::ranges::all_of>;

    RETROLIB_EXPORT constexpr auto AnyOf = ExtensionMethod<std::ranges::any_of>;

    RETROLIB_EXPORT constexpr auto NoneOf = ExtensionMethod<std::ranges::none_of>;

    RETROLIB_EXPORT constexpr auto ForEach = ExtensionMethod<std::ranges::for_each>;

} // namespace Retro::Ranges