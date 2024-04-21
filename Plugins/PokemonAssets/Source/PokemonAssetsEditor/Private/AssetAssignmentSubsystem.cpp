// "Unreal Pokémon" created by Retro & Chill.


#include "AssetAssignmentSubsystem.h"
#include "PokemonAssetsSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Repositories/AssetRepository.h"

void UAssetAssignmentSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto Settings = GetDefault<UPokemonAssetsSettings>();
    AssetRepositories.Emplace(Settings->GetPokemonIconsPackageName(), Settings->GetPokemonIconRepository().TryLoad());
    AssetRepositories.Emplace(Settings->GetTrainerSpritesPackageName(),
                              Settings->GetTrainerFrontSpriteRepository().TryLoad());

    for (auto [Package, Repository] : AssetRepositories) {
        if (!AssetRepositories.Contains(Package)) {
            continue;
        }

        TArray<FAssetData> Assets;
        FAssetRegistryModule::GetRegistry().GetAssetsByPath(Package, Assets);
        for (auto &Asset : Assets) {
            AssetRepositories[Package]->RegisterAsset(Asset);
        }
    }

    auto &AssetRegistry = FAssetRegistryModule::GetRegistry();
    AssetRegistry.OnAssetAdded().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetAdded);
    AssetRegistry.OnAssetRenamed().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetRenamed);
    AssetRegistry.OnAssetRemoved().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetRemoved);
}

void UAssetAssignmentSubsystem::HandleAssetAdded(const FAssetData &AssetData) {
    if (AssetRepositories.Contains(AssetData.PackagePath)) {
        AssetRepositories[AssetData.PackagePath]->RegisterAsset(AssetData);
    }
}

void UAssetAssignmentSubsystem::HandleAssetRenamed(const FAssetData &AssetData, const FString &) {
    if (AssetRepositories.Contains(AssetData.PackagePath)) {
        AssetRepositories[AssetData.PackagePath]->RegisterAsset(AssetData);
    }
}

void UAssetAssignmentSubsystem::HandleAssetRemoved(const FAssetData &AssetData) {
    if (AssetRepositories.Contains(AssetData.PackagePath)) {
        AssetRepositories[AssetData.PackagePath]->UnregisterAsset(AssetData);
    }
}