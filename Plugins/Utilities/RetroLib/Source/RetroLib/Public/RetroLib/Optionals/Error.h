// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals
{
    struct FErrorInvoker
    {
        template <ExpectedType O>
        constexpr decltype(auto) operator()(O &&Optional) const
        {
            return GetError(std::forward<O>(Optional));
        }
    };

    RETROLIB_EXPORT constexpr auto Error = ExtensionMethod<FErrorInvoker{}>();
} // namespace Retro::Optionals
