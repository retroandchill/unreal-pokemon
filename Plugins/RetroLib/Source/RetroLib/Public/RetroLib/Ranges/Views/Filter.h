/**
 * @file Filter.h
 * @brief Specialized constexpr filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include <ranges>
#endif

#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/FunctionTraits.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::views::filter, Filter)

} // namespace retro::ranges::views