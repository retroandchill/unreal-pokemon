/**
 * @file NameAliases.h
 * @brief Aliases for STL range operations.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include <algorithm>
#include <ranges>
#endif

#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Ranges/FeatureBridge.h"


#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

  RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::all_of, AllOf)

  RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::any_of, AnyOf)

  RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::none_of, NoneOf)

  RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, std::ranges::for_each, ForEach)

}