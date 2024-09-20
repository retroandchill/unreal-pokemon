// "Unreal Pokémon" created by Retro & Chill.

#include "Graphics/SpriteLoader.h"
#include "Graphics/AssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"
#include "Trainers/TrainerType.h"
#include <cmath>

UPaperFlipbook *USpriteLoader::GetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack) {
    return GetSpeciesBattleSprite(Pokemon->GetSpecies().ID, bBack,
                                  {.Gender = Pokemon->GetGender(), .bShiny = Pokemon->IsShiny()});
}

UPaperFlipbook *USpriteLoader::GetSpeciesBattleSprite(FName Species, bool bBack,
                                                      const FPokemonAssetParams &AdditionalParams) {
    auto SpriteResolutionList =
        CreatePokemonSpriteResolutionList(Species, AdditionalParams, bBack ? TEXT("Back") : TEXT("Front"));
    return Pokemon::Assets::Graphics::PokemonSprites.ResolveAsset(SpriteResolutionList).GetPtrOrNull();
}

UPaperFlipbook *USpriteLoader::GetPokemonIcon(const TScriptInterface<IPokemon> &Pokemon) {
    return GetSpeciesIcon(Pokemon->GetSpecies().ID, {.Gender = Pokemon->GetGender()});
}

UPaperFlipbook *USpriteLoader::GetSpeciesIcon(FName Species, const FPokemonAssetParams &AdditionalParams) {
    auto SpriteResolutionList = CreatePokemonSpriteResolutionList(Species, AdditionalParams, TEXT("Icons"));
    return Pokemon::Assets::Graphics::PokemonSprites.ResolveAsset(SpriteResolutionList).GetPtrOrNull();
}

TArray<FString> USpriteLoader::CreatePokemonSpriteResolutionList(FName Species, const FPokemonAssetParams &Params,
                                                                 FStringView Subfolder) {
    auto SubfolderString = FString::Format(TEXT("{0}/"), {Subfolder});
    auto ShinyExtension = FString::Format(TEXT("{0}Shiny/"), {Subfolder});
    auto FormExtension = FString::Format(TEXT("_{0}"), {{Params.Form.ToString()}});
    auto SpeciesExtension = Species.ToString();
    TArray<std::tuple<int32, FStringView, FStringView>> Factors;
    if (Params.bShiny) {
        Factors.Emplace(4, ShinyExtension, SubfolderString);
    }
    if (Params.bShadow) {
        Factors.Emplace(3, TEXT("_shadow"), TEXT(""));
    }
    if (Params.Gender == EPokemonGender::Female) {
        Factors.Emplace(2, TEXT("_female"), TEXT(""));
    }
    if (!Params.Form.IsNone()) {
        Factors.Emplace(1, FormExtension, TEXT(""));
    }
    Factors.Emplace(0, SpeciesExtension, TEXT("000"));

    TArray<FString> FormattedStrings;
    auto FactorPower = static_cast<int32>(std::pow(2, Factors.Num()));
    for (int i = 0; i < FactorPower; i++) {
        FStringView TrySpecies;
        FStringView TryForm;
        FStringView TryGender;
        FStringView TryShadow;
        FStringView TrySubfolder = SubfolderString;
        for (int j = 0; j < Factors.Num(); j++) {
            const auto &[FactorIndex, Extension, Fallback] = Factors[j];
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
        auto FormattedName =
            FString::Format(TEXT("{0}{1}{2}{3}{4}"), {TrySubfolder, TrySpecies, TryForm, TryGender, TryShadow});
        FormattedStrings.Add(MoveTemp(FormattedName));
    }

    return FormattedStrings;
}

UPaperFlipbook *USpriteLoader::GetTrainerSprite(const TScriptInterface<ITrainer> &Trainer, bool bBack) {
    TArray<FString, TInlineAllocator<2>> StringParts = {bBack ? TEXT("Back") : TEXT("Front"),
                                                        Trainer->GetTrainerType().ID.ToString()};
    auto JoinedString = FString::Join(StringParts, TEXT("/"));
    return Pokemon::Assets::Graphics::TrainerSprites.LoadAsset(JoinedString).GetPtrOrNull();
}