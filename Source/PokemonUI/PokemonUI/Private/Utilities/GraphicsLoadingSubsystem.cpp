// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/GraphicsLoadingSubsystem.h"

#include "PokemonUISettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"

void UGraphicsLoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	auto Settings = GetDefault<UPokemonUISettings>();
	PokemonIconsPackageName = Settings->GetPokemonIconsPackageName();
	PokemonIconsBaseMaterial = Cast<UMaterialInterface>(Settings->GetPokemonIconsBaseMaterial().TryLoad());
	IconSourceTexturePropertyName = Settings->GetIconSourceTexturePropertyName();
	IconFrameRatePropertyName = Settings->GetIconFrameRatePropertyName();
}

UMaterialInstanceDynamic* UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon& Pokemon, UObject* Outer) {
	return GetPokemonIcon(Pokemon.GetSpecies().ID, Outer);
}

UMaterialInstanceDynamic* UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species, UObject* Outer) {
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// TODO: Add support for alternate forms, gender differences, etc.
	auto AssetPath = FString::Format(TEXT("{Path}/{AssetName}.{AssetName}"), {
		                                 {TEXT("PATH"), PokemonIconsPackageName},
		                                 {TEXT("AssetName"), Species.ToString()}
	                                 });
	auto AssetData = AssetRegistryModule.GetRegistry().GetAssetByObjectPath(AssetPath);
	auto Texture = Cast<UTexture2D>(AssetData.GetAsset());
	GUARD(Texture == nullptr, nullptr)

	auto Material = UMaterialInstanceDynamic::Create(PokemonIconsBaseMaterial, Outer);
	Material->SetTextureParameterValue(IconSourceTexturePropertyName, Texture);
	return Material;
}
