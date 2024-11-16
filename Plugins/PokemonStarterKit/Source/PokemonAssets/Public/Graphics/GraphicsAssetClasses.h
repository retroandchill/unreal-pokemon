// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Assets/AssetClass.h"
#include "BattleRender.h"
#include "Images/ImageAsset.h"

/**
 * Settings for all Pokémon graphical assets
 */
namespace Pokemon::Assets::Graphics {

    /**
     * Asset class for Pokémon battle sprites.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, PokemonBattleSprites, FBattleRender);

    /**
     * Asset class for Pokémon battle sprites.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, PokemonMenuSprites, FImageAsset);

    /**
     * Asset class for Trainer sprites.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TrainerSprites, FBattleRender);

    /**
     * Asset class for type icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TypeIcons, FImageAsset);

    /**
     * Asset class for type icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, TypeIconsSmall, FImageAsset);

    /**
     * Asset class for status effect icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, StatusIcons, FImageAsset);

    /**
     * Asset class for Poké Ball icons on the summary screen.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, SummaryBalls, FImageAsset);

    /**
     * Asset class for item icons.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, ItemIcons, FImageAsset);

} // namespace Pokemon::Assets::Graphics