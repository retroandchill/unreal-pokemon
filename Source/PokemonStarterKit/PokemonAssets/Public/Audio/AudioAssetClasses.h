// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Assets/AssetClass.h"
#include "Sound/SoundBase.h"

/**
 * Namespace for anything related to audio assets.
 */
namespace Pokemon::Assets::Audio
{

    /**
     * Asset class for Pokémon cries.
     */
    UE_DECLARE_ASSET_CLASS_EXPORTED(POKEMONASSETS_API, PokemonCries, USoundBase);

} // namespace Pokemon::Assets::Audio
