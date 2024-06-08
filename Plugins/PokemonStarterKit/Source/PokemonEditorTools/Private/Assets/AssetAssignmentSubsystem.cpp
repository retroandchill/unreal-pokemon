// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetAssignmentSubsystem.h"
#include "Assets/AssetAssignmentSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Repositories/AssetRepository.h"
#include "Repositories/StaticImageRepository.h"
#include "Repositories/TextureRepository.h"
#include "Settings/BaseSettings.h"

void UAssetAssignmentSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto AssetAssignmentSettings = GetDefault<UAssetAssignmentSettings>();
    auto &Settings = Pokemon::FBaseSettings::Get().GetSpriteRepositories();
    AssetRepositories.Emplace(AssetAssignmentSettings->GetPokemonSpritePackageName(),
                              Settings.PokemonSpriteRepository.TryLoad());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetTrainerSpritesPackageName(),
                              Settings.TrainerFrontSpriteRepository.TryLoad());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetTypeIconsPackageName(),
                              Settings.TypeIconRepository.TryLoad());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetStatusIconsPackageName(),
                              Settings.StatusIconRepository.TryLoad());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetSummaryBallPackageName(),
                              Settings.SummaryBallRepository.TryLoad());
    AssetRepositories.Emplace(AssetAssignmentSettings->GetItemIconPackageName(),
                              Settings.ItemIconRepository.TryLoad());

    for (const auto &[Package, Repository] : AssetRepositories) {
        if (Repository == nullptr) {
            continue;
        }
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