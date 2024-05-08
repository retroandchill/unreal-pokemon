// "Unreal Pokémon" created by Retro & Chill.

#include "AssetAssignmentSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PokemonAssetsSettings.h"
#include "Repositories/AssetRepository.h"

void UAssetAssignmentSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto Settings = GetDefault<UPokemonAssetsSettings>();
    AssetRepositories.Emplace(Settings->GetPokemonSpritePackageName(), Settings->GetPokemonSpriteRepository().TryLoad());
    AssetRepositories.Emplace(Settings->GetTrainerSpritesPackageName(),
                              Settings->GetTrainerFrontSpriteRepository().TryLoad());

    for (const auto &[Package, Repository] : AssetRepositories) {
        Repository->SetBasePackage(Package.ToString());
        if (!AssetRepositories.Contains(Package)) {
            continue;
        }

        TArray<FAssetData> Assets;
        FAssetRegistryModule::GetRegistry().GetAssetsByPath(Package, Assets, true);
        for (const auto &Asset : Assets) {
            AssetRepositories[Package]->RegisterAsset(Asset);
        }
    }

    auto &AssetRegistry = FAssetRegistryModule::GetRegistry();
    AssetRegistry.OnAssetAdded().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetAdded);
    AssetRegistry.OnAssetRenamed().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetRenamed);
    AssetRegistry.OnAssetRemoved().AddUObject(this, &UAssetAssignmentSubsystem::HandleAssetRemoved);
}

void UAssetAssignmentSubsystem::HandleAssetAdded(const FAssetData &AssetData) {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->RegisterAsset(AssetData);
            break;
        }
    }
}

void UAssetAssignmentSubsystem::HandleAssetRenamed(const FAssetData &AssetData, const FString &) {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->RegisterAsset(AssetData);
            break;
        }
    }
}

void UAssetAssignmentSubsystem::HandleAssetRemoved(const FAssetData &AssetData) {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->UnregisterAsset(AssetData);
            break;
        }
    }
}