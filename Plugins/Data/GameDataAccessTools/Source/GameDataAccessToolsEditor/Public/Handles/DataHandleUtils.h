// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Base namespace for GameDataAccessTools utility functions.
 */
namespace GameData
{
    /**
     * Determines whether the given UScriptStruct represents a valid Game Data Handle structure.
     *
     * This function checks if the provided UScriptStruct instance meets the criteria for being
     * identified as a Game Data Handle structure. Specifically, it ensures the structure is
     * of type UCSScriptStruct, contains a property named "ID" of FNameProperty type, and references
     * a valid managed type handle via its managed type information.
     *
     * @param Struct The UScriptStruct instance to check.
     * @return True if the given UScriptStruct is a valid Game Data Handle structure; otherwise, false.
     */
    bool IsDataHandleStruct(UScriptStruct *Struct);
} // namespace GameData
