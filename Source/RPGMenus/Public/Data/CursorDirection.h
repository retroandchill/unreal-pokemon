// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

/**
 * The direction of the cursor based upon the key received
 */
UENUM(BlueprintType)
enum class ECursorDirection : uint8 {
	/**
	 * Cursor UP
	 */
	Up,
	/**
	 * Cursor DOWN
	 */
	Down,

	/**
	 * Cursor LEFT
	 */
	Left,
	/**
	 * Cursor RIGHT
	 */
	Right
};
