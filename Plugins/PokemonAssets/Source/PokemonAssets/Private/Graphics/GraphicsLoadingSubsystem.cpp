// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/GraphicsLoadingSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Pokemon/Pokemon.h"
#include "PokemonAssetsSettings.h"
#include "Algo/Unique.h"
#include "Repositories/TextureRepository.h"
#include "Species/SpeciesData.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"

#include <cmath>

TArray<FName> CreatePokemonSpriteResolutionList(FName Species,
    const FPokemonAssetParams &Params, FStringView Subfolder) {
    auto SubfolderString = FString::Format(TEXT("{0}/"), {Subfolder});
    auto ShinyExtension = FString::Format(TEXT("{0}Shiny/"), {Subfolder});
    auto SpeciesExtension = Species.ToString();
    auto FormExtension = FString::Format(TEXT("_{0}"), {{Params.Form}});
    auto GenderExtension = Params.Gender == EPokemonGender::Female ? TEXT("_female") : TEXT("");
    auto ShadowExtension = Params.bShadow ? TEXT("_shadow") : TEXT("");
    TArray<std::tuple<int32, FStringView, FStringView>> Factors;
    Factors.Emplace(4, ShinyExtension, SubfolderString);
    Factors.Emplace(3, ShadowExtension, TEXT(""));
    Factors.Emplace(2, GenderExtension, TEXT(""));
    Factors.Emplace(1, FormExtension, TEXT(""));
    Factors.Emplace(0, SpeciesExtension, TEXT("000"));

    TArray<FString> FormattedStrings;
    auto FactorPower = static_cast<int32>(std::pow(2, Factors.Num()));
    for (int i = 0; i < FactorPower; i++) {
        FStringView TrySpecies;
        FStringView TryForm;
        FStringView TryGender;
        FStringView TryShadow;
        FStringView TrySubfolder;
        for (int j = 0; j < Factors.Num(); j++) {
            auto &[FactorIndex, Extension, Fallback] = Factors[j];
            auto TestValue = i / static_cast<int32>(std::pow(2, j)) % 2 == 0 ? Extension : Fallback;
            switch (FactorIndex) {
            case 0:
                TrySpecies = TestValue;
                break;
            case 1:
                TryForm = TestValue;
                break;
            case 2:
                TryGender = TestValue;
                break;
            case 3:
                TryShadow = TestValue;
                break;
            default:
                TrySubfolder = TestValue;
                break;
            }
        }
        auto FormattedName = FString::Format(TEXT("{0}{1}{2}{3}{4}"),
                {TrySubfolder, TrySpecies, TryForm, TryGender, TryShadow}
            );
        FormattedStrings.Add(MoveTemp(FormattedName));
    }
    
    TArray<FName> Ret;
    TSet<FStringView> Explored;
    for (const auto &Name : FormattedStrings) {
        if (Explored.Contains(Name)) {
            continue;
        }
        
        Explored.Add(Name);
        Ret.Emplace(*Name);
    }
    return Ret;
}

void UGraphicsLoadingSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    auto Settings = GetDefault<UPokemonAssetsSettings>();
    PokemonIconsRepository = Settings->GetPokemonSpriteRepository();
    PokemonIconsBaseMaterial = Cast<UMaterialInterface>(Settings->GetPokemonIconsBaseMaterial().TryLoad());
    IconSourceTexturePropertyName = Settings->GetIconSourceTexturePropertyName();
    IconFrameRatePropertyName = Settings->GetIconFrameRatePropertyName();

    TrainerSpritesRepository = Settings->GetTrainerFrontSpriteRepository();
    TrainerSpriteBaseMaterial = Cast<UMaterialInterface>(Settings->GetTrainerSpriteBaseMaterial().TryLoad());
    TrainerSpriteSourceTexturePropertyName = Settings->GetTrainerSpriteSourceTexturePropertyName();
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetPokemonBattleSprite(const IPokemon &Pokemon,
    UObject *Outer, bool bBack) {
    return GetPokemonBattleSprite(Pokemon.GetSpecies().ID, Outer, bBack, {
        .Gender = Pokemon.GetGender()
    });
}

TPair<UMaterialInstanceDynamic *, FVector2D> UGraphicsLoadingSubsystem::GetPokemonBattleSprite(FName Species,
    UObject *Outer, bool bBack, const FPokemonAssetParams &AdditionalParams) {
    auto SpriteResolutionList = CreatePokemonSpriteResolutionList(Species, AdditionalParams,
        bBack ? TEXT("Back") : TEXT("Front"));
    auto Texture = PokemonIconsRepository.Get()->ResolveAsset(SpriteResolutionList);
    if (Texture == nullptr) {
        return {nullptr, FVector2D()};
    }

    auto Material = UMaterialInstanceDynamic::Create(PokemonIconsBaseMaterial, Outer);
    Material->SetTextureParameterValue(IconSourceTexturePropertyName, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}

UMaterialInstanceDynamic *UGraphicsLoadingSubsystem::GetPokemonIcon(const IPokemon &Pokemon, UObject *Outer) {
    return GetPokemonIcon(Pokemon.GetSpecies().ID, Outer, {
        .Gender = Pokemon.GetGender()
    });
}

UMaterialInstanceDynamic * UGraphicsLoadingSubsystem::GetPokemonIcon(FName Species, UObject *Outer,
    const FPokemonAssetParams &AdditionalParams) {
    auto SpriteResolutionList = CreatePokemonSpriteResolutionList(Species, AdditionalParams, TEXT("Icons"));
    auto Texture = PokemonIconsRepository.Get()->ResolveAsset(SpriteResolutionList);
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
        return {nullptr, FVector2D()};
    }

    auto Material = UMaterialInstanceDynamic::Create(TrainerSpriteBaseMaterial, Outer);
    Material->SetTextureParameterValue(TrainerSpriteSourceTexturePropertyName, Texture);
    return {Material, FVector2D(Texture->GetSizeY(), Texture->GetSizeY())};
}
