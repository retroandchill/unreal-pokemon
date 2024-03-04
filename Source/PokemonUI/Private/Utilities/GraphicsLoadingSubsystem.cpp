// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/GraphicsLoadingSubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"

UObject* UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon& Pokemon) {
	return GetPokemonIcon(Pokemon.GetSpecies().ID);
}

UObject* UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species) {
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// TODO: Add support for alternate forms, gender differences, etc.
	auto AssetPath = FString::Format(TEXT("{Path}/{AssetName}.{AssetName}"), {
		{TEXT("PATH"), PokemonIconsPackageName},
		{TEXT("AssetName"), Species.ToString()}
	});
	auto AssetData = AssetRegistryModule.GetRegistry().GetAssetByObjectPath(AssetPath);
	return AssetData.GetAsset();	
}
