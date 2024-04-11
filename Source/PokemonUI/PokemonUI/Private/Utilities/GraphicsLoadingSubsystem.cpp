// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/GraphicsLoadingSubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Pokemon/Pokemon.h"
#include "PokemonUISettings.h"
#include "Species/SpeciesData.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"

void UGraphicsLoadingSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto Settings = GetDefault<UPokemonUISettings>();
    PokemonIconsPackageName = Settings->GetPokemonIconsPackageName();
    PokemonIconsBaseMaterial = Cast<UMaterialInterface>(Settings->GetPokemonIconsBaseMaterial().TryLoad());
    IconSourceTexturePropertyName = Settings->GetIconSourceTexturePropertyName();
    IconFrameRatePropertyName = Settings->GetIconFrameRatePropertyName();

    TrainerSpritesPackageName = Settings->GetTrainerSpritesPackageName();
    TrainerSpriteBaseMaterial = Cast<UMaterialInterface>(Settings->GetTrainerSpriteBaseMaterial().TryLoad());
    TrainerSpriteSourceTexturePropertyName = Settings->GetTrainerSpriteSourceTexturePropertyName();
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon &Pokemon, UObject *Outer) {
    return GetPokemonIcon(Pokemon.GetSpecies().ID, Outer);
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species, UObject *Outer) {
    auto &AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    // TODO: Add support for alternate forms, gender differences, etc.
    auto AssetPath = FString::Format(TEXT("{Path}/{AssetName}.{AssetName}"), {{TEXT("PATH"), PokemonIconsPackageName},
                                                                              {TEXT("AssetName"), Species.ToString()}});
    auto AssetData = AssetRegistryModule.GetRegistry().GetAssetByObjectPath(AssetPath);
    auto Texture = Cast<UTexture2D>(AssetData.GetAsset());
    if (Texture == nullptr) {
        return nullptr;
    }

    auto Material = UMaterialInstanceDynamic::Create(PokemonIconsBaseMaterial, Outer);
    Material->SetTextureParameterValue(IconSourceTexturePropertyName, Texture);
    return Material;
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetTrainerSprite(const ITrainer &Trainer,
                                                                                         UObject *Outer) {
    return GetTrainerSprite(Trainer.GetTrainerType().ID, Outer);
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetTrainerSprite(FName TrainerType,
                                                                                         UObject *Outer) {
    auto &AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    auto AssetPath =
        FString::Format(TEXT("{Path}/{AssetName}.{AssetName}"),
                        {{TEXT("PATH"), TrainerSpritesPackageName}, {TEXT("AssetName"), TrainerType.ToString()}});
    auto AssetData = AssetRegistryModule.GetRegistry().GetAssetByObjectPath(AssetPath);
    auto Texture = Cast<UTexture2D>(AssetData.GetAsset());
    if (Texture == nullptr) {
        return {};
    }

    auto Material = UMaterialInstanceDynamic::Create(TrainerSpriteBaseMaterial, Outer);
    Material->SetTextureParameterValue(TrainerSpriteSourceTexturePropertyName, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}
