/**
 * @file RetroLib.ixx
 * @brief Module declaration for RetroLib.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
module;

#include <typeinfo>

export module RetroLib;

import std;

#define RETROLIB_EXPORT export

#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Concepts/OpaqueStorage.h"

#include "RetroLib/Utils/Polymorphic.h"