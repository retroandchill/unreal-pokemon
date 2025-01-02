/**
 * @file OpaqueStorage.h
 * @brief Contains the concept definitions for small buffer optimization with opaque storage unions.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/Inheritance.h"

#if !RETROLIB_WITH_MODULES
#include <array>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * Global constant used to determine the size of the small buffer for opaque storage types.
     *
     * @note Some types have an optional template parameter that allows the small storage size to be
     * changed to something else if the default size is too much or too little. This is helpful for
     * classes that use templated type erasure where the size of the erased type is consistent.
     */
    RETROLIB_EXPORT constexpr size_t DEFAULT_SMALL_STORAGE_SIZE = sizeof(void *) * 7;
} // namespace retro
