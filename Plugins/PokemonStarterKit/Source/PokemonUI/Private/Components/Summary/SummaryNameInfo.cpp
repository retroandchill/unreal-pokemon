﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryNameInfo.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Utilities/PokemonUIUtils.h"
#include "Utilities/WidgetUtilities.h"

void USummaryNameInfo::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    using enum ESlateVisibility;

    Super::Refresh_Implementation(Pokemon);
    PokemonNameText->SetText(Pokemon->GetNickname());

    auto Gender = Pokemon->GetGender();
    UPokemonUIUtils::SetPokemonGenderText(Gender, PokemonGenderText);
    if (GenderTextColors.Contains(Gender)) {
        PokemonGenderText->SetTextStyle(GenderTextColors[Gender]);
    }

    PokemonLevelText->SetText(FText::FromString(FString::FromInt(Pokemon->GetStatBlock()->GetLevel())));

    // TODO: Configure the status and Poké Ball
    // clang-format off
    auto BallIcon = Pokemon::Assets::Graphics::SummaryBalls.LoadAsset(Pokemon->GetPokeBall()) |
                    UE::Optionals::FlatMap([](const FImageAsset &ImageAsset) { return ImageAsset.TryGet(); }) |
                    UE::Optionals::GetPtrOrNull;
    // clang-format on
    UWidgetUtilities::SetBrushFromAsset(PokemonBallIcon, BallIcon, true);
    PokemonStatusIcon->SetVisibility(Hidden);
}