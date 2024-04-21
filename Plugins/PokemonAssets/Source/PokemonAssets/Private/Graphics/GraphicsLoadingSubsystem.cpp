// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/GraphicsLoadingSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Pokemon/Pokemon.h"
#include "PokemonAssetsSettings.h"
#include "Repositories/TextureRepository.h"
#include "Species/SpeciesData.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"

void UGraphicsLoadingSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto Settings = GetDefault<UPokemonAssetsSettings>();
    PokemonIconsRepository = Settings->GetPokemonIconRepository();
    PokemonIconsBaseMaterial = Cast<UMaterialInterface>(Settings->GetPokemonIconsBaseMaterial().TryLoad());
    IconSourceTexturePropertyName = Settings->GetIconSourceTexturePropertyName();
    IconFrameRatePropertyName = Settings->GetIconFrameRatePropertyName();

    TrainerSpritesRepository = Settings->GetTrainerFrontSpriteRepository();
    TrainerSpriteBaseMaterial = Cast<UMaterialInterface>(Settings->GetTrainerSpriteBaseMaterial().TryLoad());
    TrainerSpriteSourceTexturePropertyName = Settings->GetTrainerSpriteSourceTexturePropertyName();
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon &Pokemon, UObject *Outer) {
    return GetPokemonIcon(Pokemon.GetSpecies().ID, Outer);
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species, UObject *Outer) {
    FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    // TODO: Add support for alternate forms, gender differences, etc.
    auto Texture = PokemonIconsRepository.Get()->FetchAsset(Species);
    if (Texture == nullptr) {
        return nullptr;
    }

    auto Material = UMaterialInstanceDynamic::Create(PokemonIconsBaseMaterial, Outer);
    Material->SetTextureParameterValue(IconSourceTexturePropertyName, Texture);
    return Material;
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetTrainerSprite(const ITrainer &Trainer,
                                                                                         UObject *Outer) const {
    return GetTrainerSprite(Trainer.GetTrainerType().ID, Outer);
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetTrainerSprite(FName TrainerType,
                                                                                         UObject *Outer) const {
    auto Texture = TrainerSpritesRepository.Get()->FetchAsset(TrainerType);
    if (Texture == nullptr) {
        return {};
    }

    auto Material = UMaterialInstanceDynamic::Create(TrainerSpriteBaseMaterial, Outer);
    Material->SetTextureParameterValue(TrainerSpriteSourceTexturePropertyName, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}
