// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalRef.h"

namespace UE::Optionals {
    /**
     * Creates a TOptional object containing the provided value.
     *
     * @param Value The value to be wrapped in a TOptional. Can be either an lvalue or rvalue reference.
     * @return A TOptional containing the provided value. If the input is an lvalue reference,
     *         the TOptional will store an lvalue reference. Otherwise, it will store a decayed version of the value.
     */
    template <typename T>
    auto Of(T&& Value) {
        if constexpr (std::is_lvalue_reference_v<T>) {
            return TOptional<T>(std::forward<T>(Value));
        } else {
            return TOptional<std::decay_t<T>>(std::forward<T>(Value));
        }
    }

    /**
     * Creates a TOptional object containing a raw pointer to the provided value.
     *
     * @param Value The value to be converted to a raw pointer and wrapped in a TOptional.
     *              Can be either an lvalue or rvalue reference.
     * @return A TOptional containing a raw pointer to the provided value. If the input is a pointer,
     *         the TOptional will store a reference to the pointed-to value.
     */
    template <typename T, typename R = Ranges::TRawPointerType<T>>
        requires Ranges::Pointer<T>
    auto Of(T&& Value) {
        auto RawPointer = Ranges::GetRawPointer<T>(std::forward<T>(Value));
        check(RawPointer != nullptr);
        return TOptional<std::remove_pointer_t<R>&>(RawPointer);
    }

    
    /**
     * Creates a TOptional object containing a valid subclass of T.
     *
     * @param Class The TSubclassOf object to be wrapped in a TOptional. Must not be null.
     * @return A TOptional containing a TNonNullSubclassOf object derived from Class.
     */
    template <typename T>
        requires std::derived_from<T, UObject>
    auto Of(const TSubclassOf<T> &Class) {
        check(Class != nullptr);
        return TOptional<TNonNullSubclassOf<T>>(Class);
    }
    
}
