// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

/**
 * Represents the type of Egg Group
 */
UENUM(BlueprintType)
enum class EEggGroupType : uint8 {
	/**
	 * Eggs are created when two Pokémon of the same Egg Group are in the Day Care together
	 */

	Normal,
	/**
	 * A Pokémon in this Egg Group will create Eggs with any Pokémon in a Normal group, creating an Egg of the other parent
	 */
	Ditto,

	/**
	 * This species cannot produce Eggs unless a special rider is added for that species
	 */
	NoEggs
};
