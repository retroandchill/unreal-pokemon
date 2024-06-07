// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetAssignmentSubsystem.h"
#include "Assets/AssetAssignmentSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Repositories/AssetRepository.h"
#include "Repositories/StaticImageRepository.h"
#include "Repositories/TextureRepository.h"
#include "Settings/AssetLoaderSettings.h"

void UAssetAssignmentSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto AssetAssignmentSettings = GetDefault<UAssetAssignmentSettings>();
    auto AssetLoaderSettings = GetDefault<UAssetLoaderSettings>();
    AssetRepositories.Emplace(AssetAssignmentSettings->GetPokemonSpritePackageName(),
                              AssetLoaderSettings->GetPokemonSpriteRepository());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetTrainerSpritesPackageName(),
                              AssetLoaderSettings->GetTrainerFrontSpriteRepository());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetTypeIconsPackageName(),
                              AssetLoaderSettings->GetTypeIconRepository());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetStatusIconsPackageName(),
                              AssetLoaderSettings->GetStatusIconRepository());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetSummaryBallPackageName(),
                              AssetLoaderSettings->GetSummaryBallRepository());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetItemIconPackageName(),
                              AssetLoaderSettings->GetItemIconRepository());

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

void UAssetAssignmentSubsystem::HandleAssetAdded(const FAssetData &AssetData) const {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->RegisterAsset(AssetData);
            break;
        }
    }
}

void UAssetAssignmentSubsystem::HandleAssetRenamed(const FAssetData &AssetData, const FString &) const {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->RegisterAsset(AssetData);
            break;
        }
    }
}

void UAssetAssignmentSubsystem::HandleAssetRemoved(const FAssetData &AssetData) const {
    for (auto &[Package, Repository] : AssetRepositories) {
        if (AssetData.PackagePath.ToString().Contains(Package.ToString())) {
            Repository->UnregisterAsset(AssetData);
            break;
        }
    }
}