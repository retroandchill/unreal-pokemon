// "Unreal Pokémon" created by Retro & Chill.

#include "Audio/AudioLoader.h"
#include "Audio/AssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"

USoundBase *UAudioLoader::FindPokemonCry(const TScriptInterface<IPokemon> &Pokemon) {
    return FindSpeciesCry(Pokemon->GetSpecies().ID, {.Gender = Pokemon->GetGender(), .bShiny = false});
}

USoundBase *UAudioLoader::FindSpeciesCry(const FSpeciesHandle &Species, const FPokemonAssetParams &AdditionalParams) {
    auto CryResolutionList = AdditionalParams.CreateResolutionList(Species);
    return Pokemon::Assets::Audio::PokemonCries.ResolveAsset(CryResolutionList).GetPtrOrNull();
}