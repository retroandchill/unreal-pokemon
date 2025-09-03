// "Unreal Pokémon" created by Retro & Chill.

#pragma once

/**
 * Blueprint function library used for generating mock data for testing purposes
 */
class POKEMONTESTS_API FPokemonTestUtilities {

  public:
    /**
     * Create a mock party for the testing purposes. The party in question contains Pokémon with the following stats
     * <p>Samurott (M) @ Life Orb</p>
     * <p>Ability: Torrent</p>
     * <p>Tera Type: Ground</p>
     * <p>EVs: 164 Atk / 92 SpA / 252 Spe</p>
     * <p>Naive Nature</p>
     * <p>- Hydro Pump</p>
     * <p>- Knock Off</p>
     * <p>- Megahorn</p>
     * <p>- Sacred Sword</p>
     * <p></p>
     * <p>Emboar (F) @ Choice Band</p>
     * <p>Ability: Reckless</p>
     * <p>Shiny: Yes</p>
     * <p>Tera Type: Fire</p>
     * <p>EVs: 252 Atk / 4 Def / 252 Spe</p>
     * <p>Jolly Nature</p>
     * <p>- Flare Blitz</p>
     * <p>- Superpower</p>
     * <p>- Wild Charge</p>
     * <p>- Earthquake</p>
     * <p></p>
     * <p>Serperior (M) @ Rocky Helmet</p>
     * <p>Ability: Contrary</p>
     * <p>Tera Type: Grass</p>
     * <p>EVs: 208 HP / 48 Def / 252 Spe</p>
     * <p>Timid Nature</p>
     * <p>IVs: 0 Atk</p>
     * <p>- Leaf Storm</p>
     * <p>- Taunt</p>
     * <p>- Glare</p>
     * <p>- Defog  </p>
     *
     * @param WorldContext The object used to obtain the world context
     */
    static void CreateMockParty(UObject *WorldContext);
};