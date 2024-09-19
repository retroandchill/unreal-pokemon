// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Assets/AssetClass.h"

/**
 * Settings for all Pokémon graphical assets
 */
namespace Pokemon::Assets::Graphics {

    /**
     * Asset class for Pokémon battle sprites.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, PokemonSprites, UPaperFlipbook);

    /**
     * Asset class for Trainer sprites.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TrainerSprites, UPaperFlipbook);

    /**
     * Asset class for type icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TypeIcons, UObject);

    /**
     * Asset class for typed move panels in battle.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TypePanels, UObject);

    /**
     * Asset class for status effect icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, StatusIcons, UObject);

    /**
     * Asset class for Poké Ball icons on the summary screen.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, SummaryBalls, UObject);

    /**
     * Asset class for item icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, ItemIcons, UObject);
    
}