// "Unreal Pokémon" created by Retro & Chill.

#pragma once


#include "RetroLib/Concepts/Pointers.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/TypeTraits.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    struct FErrorPtrOrNullInvoker {

        template <ExpectedType O>
        constexpr auto operator()(O &&Optional) const {
            return HasValue(std::forward<O>(Optional)) ? nullptr : GetPtrFromRef(GetError(std::forward<O>(Optional)));
        }
    };
    
    RETROLIB_EXPORT constexpr auto ErrorPtrOrNull = ExtensionMethod<FErrorPtrOrNullInvoker{}>();

}