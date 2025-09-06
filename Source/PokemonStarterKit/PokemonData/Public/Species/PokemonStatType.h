// "Unreal Pokémon" created by Retro & Chill.
#pragma once

/**
 * Represents the type of stat this stat represents
 */
UENUM(BlueprintType)
enum class EPokemonStatType : uint8
{
    /**
     * Represents a stat that is important everywhere and is actively displayed to the player
     */
    Main,

    /**
     * Represents a stat that is only really important during battle, but is still displayed to the player
     */
    MainBattle,

    /**
     * Represents a stat that only exists during battle and is invisible to the player beyond how many stages it's been
     * raised/lowered
     */
    Battle
};
