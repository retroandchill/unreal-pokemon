/**
 * @file RetroLib.ixx
 * @brief Module declaration for RetroLib.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
module;

#include "RetroLib/RetroLibMacros.h"

#include <bit>
#include <cassert>
#include <ranges>
#include <typeinfo>

export module RetroLib;

import std;

#define RETROLIB_EXPORT export

// clang-format off
#include "RetroLib/TypeTraits.h"
#include "RetroLib/FunctionTraits.h"

#include "RetroLib/TypeTraits.h"

#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Concepts/OpaqueStorage.h"
#include "RetroLib/Concepts/ParameterPacks.h"

#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Functional/BindFront.h"
#include "RetroLib/Functional/BindMethod.h"
#include "RetroLib/Functional/ExtensionMethods.h"

#include "RetroLib/Utils/ForwardLike.h"
#include "RetroLib/Utils/Polymorphic.h"

#include "RetroLib/Optionals/OptionalOperations.h"

#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/Ranges/Concepts/Containers.h"

#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/AnyView.h"

// clang-format on
