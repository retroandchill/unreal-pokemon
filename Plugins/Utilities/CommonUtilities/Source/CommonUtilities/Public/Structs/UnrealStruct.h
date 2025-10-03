// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Concept for any USTRUCT that is defined using native information.
 */
template <typename T>
concept CoreStructType = requires {
    { TBaseStructure<std::remove_cvref_t<T>>::Get() } -> std::same_as<UScriptStruct *>;
};

/**
 * Concept for any USTRUCT that is defined using source generated bodies.
 */
template <typename T>
concept DeclaredStruct = requires {
    { std::remove_cvref_t<T>::StaticStruct() } -> std::same_as<UScriptStruct *>;
};

/**
 * Concept for any USTRUCT in the editor.
 */
template <typename T>
concept UEStruct = CoreStructType<T> || DeclaredStruct<T>;

/**
 * Retrieves the UScriptStruct corresponding to the template type T.
 * If the type T is a DeclaredStruct, it retrieves the static struct associated with T.
 * Otherwise, retrieves the base structure for the type T.
 *
 * @tparam T The struct type to retrieve
 * @return A pointer to the UScriptStruct representing the structure associated with the type T.
 */
template <UEStruct T>
constexpr UScriptStruct *GetScriptStruct()
{
    if constexpr (DeclaredStruct<T>)
    {
        return std::remove_cvref_t<T>::StaticStruct();
    }
    else
    {
        return TBaseStructure<std::remove_cvref_t<T>>::Get();
    }
}
