// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace UE
{
    /**
     * A utility function that informs the compiler that a specific code path is unreachable.
     * The function does not return and ensures undefined behavior if the unreachable code is executed.
     *
     * This function utilizes compiler-specific intrinsic functions to optimize code by marking
     * sections as unreachable for MSVC and GCC/Clang compilers.
     *
     * Usage scenarios:
     * - Marking code branches that should logically never be executed.
     * - Assisting the compiler in generating optimized code paths.
     *
     * Behavior:
     * - On MSVC (excluding Clang), __assume(false) is used.
     * - On GCC or Clang, __builtin_unreachable() is invoked.
     *
     * Note:
     * - Improper use can lead to undefined behavior.
     * - Properly annotate all branches in your code to avoid execution of this function unless logically valid.
     */
    [[noreturn]] inline void Unreachable()
    {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
#else // GCC, Clang
        __builtin_unreachable();
#endif
    }
} // namespace UE