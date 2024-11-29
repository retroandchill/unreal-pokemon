/**
 * @file RetroLib.ixx
 * @brief Module declaration for RetroLib.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
module;

#include "RetroLib/RetroLibMacros.h"

#include <cassert>
#include <typeinfo>

export module RetroLib;

import std;

#define RETROLIB_EXPORT export

// clang-format off

#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Concepts/OpaqueStorage.h"

#include "RetroLib/Utils/Polymorphic.h"

#include "RetroLib/Ranges/Views/AnyView.h"

// clang-format on
