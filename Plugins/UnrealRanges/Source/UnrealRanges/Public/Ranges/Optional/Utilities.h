// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Traits.h"

namespace UE::Optionals {
    /**
     * Retrieve a possible null value from an optional type.
     * If the optional type is a subclass, the method returns the value obtained by the `Get` method of the optional
     * with `nullptr` as a parameter. Otherwise, it returns the value obtained by the `GetPtrOrNull` method.
     *
     * @tparam T The optional type
     * @param Optional The optional object from which to retrieve the nullable value.
     * @return The nullable value contained within the optional object. The exact return type depends on the type of the
     * optional.
     */
    template <typename T>
        requires UEOptional<T>
    constexpr decltype(auto) GetNullableValue(T &&Optional) {
        if constexpr (SubclassOptional<T>) {
            return Optional.Get(nullptr);
        } else {
            return Optional.GetPtrOrNull();
        }
    }

    /**
     * The nullable value type for a given optional.
     *
     * @tparam T The optional type
     */
    template <typename T>
        requires UEOptional<T>
    using TNullableValue = decltype(GetNullableValue(std::decay_t<T>()));

} // namespace UE::Optionals
