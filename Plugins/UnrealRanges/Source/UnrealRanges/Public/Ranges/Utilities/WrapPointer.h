// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    struct FWrapPointer {
        template <typename T>
            requires std::derived_from<std::remove_cv_t<T>, UObject> || UnrealInterface<T>
        constexpr auto operator()(T *Ptr) const {
            if constexpr (std::derived_from<std::remove_cv_t<T>, UObject>) {
                return TObjectPtr<T>(Ptr);
            } else if constexpr (UnrealInterface<T>) {
                TScriptInterface<T> Interface;
                Interface.SetObject(Ptr != nullptr ? Ptr->_getUObject() : nullptr);
                Interface.SetInterface(Ptr);
                return Interface;
            }
        }
    };

    constexpr FWrapPointer WrapPointer;
}
