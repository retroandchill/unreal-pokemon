#pragma once

/**
 * Denotes one of the various encounter types that can occur in the world
 */
UENUM(BlueprintType)
enum class EEncounterCategory : uint8 {
	/**
	 * Not any of the specified encounter types. This is typically used for encounters that are resolved by some kind
	 * of scripted event. (e.i. Using a move like Rock Smash or Headbutt)
	 */
	None,

	/**
	 * An encounter on a specific type of terrain such as tall grass
	 */
	Land,

	/**
	 * An encounter that can happen on any tile of the map
	 */
	Cave,

	/**
	 * An encounter that occurs when surfing
	 */
	Water,

	/**
	 * An encounter that occurs when fishing
	 */
	Fishing,

	/**
	 * An encounter that only occurs during some kind of special event such as a Bug-Catching Contest
	 */
	Contest
};
