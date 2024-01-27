#pragma once

/**
 * Represents one of the three possible damage categories
 */
UENUM(BlueprintType)
enum class EDamageCategory : uint8
{
	/**
	 * Uses Attack and Defense to calculate damage
	 */
	Physical = 0,

	/**
	 * Uses SpAtk and SpDef to calculate damage
	 */
	Special = 1,

	/**
	 * Does not deal any direct damage
	 */
	Status = 2
};
