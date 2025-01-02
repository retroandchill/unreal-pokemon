/**
 * @file Transform.h
 * @brief Constexpr transform view adapter.
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


#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::views::transform, Transform)

} // namespace retro::ranges::views
