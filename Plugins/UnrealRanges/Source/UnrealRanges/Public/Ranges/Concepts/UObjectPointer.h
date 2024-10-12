// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {
    namespace Detail {

        template <typename T>
        struct TIsUObjectPointer : std::false_type {};

        template <typename T>
            requires std::is_base_of_v<UObject, T>
        struct TIsUObjectPointer<T *> : std::true_type {};

        template <typename T>
            requires std::is_base_of_v<UObject, T>
        struct TIsUObjectPointer<TObjectPtr<T>> : std::true_type {};

    } // namespace Detail

    template <typename T>
    concept UObjectPointer = Detail::TIsUObjectPointer<std::remove_cvref_t<T>>::value;

    template <typename T>
    concept DereferencesToUObject = requires(T Ptr) {
        { Ptr.Get() } -> std::convertible_to<const UObject *>;
    };

    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    using TVariablePtr = std::conditional_t<UnrealInterface<T>, TScriptInterface<T>, T*>;

} // namespace UE::Ranges