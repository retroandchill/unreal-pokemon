// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace UE
{
    [[noreturn]] inline void Unreachable()
    {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
#else // GCC, Clang
        __builtin_unreachable();
#endif
    }
} // namespace UE