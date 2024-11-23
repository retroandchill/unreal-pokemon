﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Pointers.h"

namespace UE::Ranges {
    /**
     * A struct used to determine if a pointer is valid.
     * The validity check varies based on the type of pointer and what it dereferences to.
     */
    struct FValidPtr {

        /**
         * Determines the validity of a given pointer based on its type and what it dereferences to.
         *
         * @param Ptr The pointer to be checked for validity.
         * @return True if the pointer is valid, false otherwise.
         */
        template <Pointer T>
        constexpr bool operator()(const T &Ptr) const {
            if constexpr (DereferencesTo<T, const UObject>) {
                return IsValid(GetRawPointer(Ptr));
            } else if constexpr (DereferencesToInterface<T>) {
                auto RawPtr = GetRawPointer(Ptr);
                if constexpr (std::derived_from<std::decay_t<T>, FScriptInterface>) {
                    return IsValid(Ptr.GetObject());
                } else {
                    return RawPtr != nullptr && IsValid(RawPtr->_getUObject());
                }
            } else {
                return Ptr != nullptr;
            }
        }
    };

    /**
     * A constant instance of FValidPtr used to check the validity of pointers.
     * This object can be used to determine if a pointer is valid based on its type and dereferenced value.
     */
    constexpr FValidPtr ValidPtr;
} // namespace UE::Ranges
