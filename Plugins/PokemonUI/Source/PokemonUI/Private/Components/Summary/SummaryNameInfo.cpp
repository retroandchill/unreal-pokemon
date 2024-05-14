// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryNameInfo.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Primatives/DisplayText.h"
#include "Utilities/PokemonUIUtils.h"
#include "Utilities/WidgetUtilities.h"

void USummaryNameInfo::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    using enum ESlateVisibility;

    Super::Refresh_Implementation(Pokemon);
    PokemonNameText->SetText(Pokemon->GetNickname());

    auto Gender = Pokemon->GetGender();
    UPokemonUIUtils::SetPokemonGenderText(*Pokemon, PokemonGenderText);
    if (GenderTextColors.Contains(Gender)) {
        UPokemonUIUtils::SetItemTextColor(PokemonGenderText, GenderTextColors[Gender]);
    }

    PokemonLevelText->SetText(FText::FromString(FString::FromInt(Pokemon->GetStatBlock()->GetLevel())));

    // TODO: Configure the status and Poké Ball
    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    UWidgetUtilities::SetBrushFromAsset(PokemonBallIcon,
                                        GraphicsLoadingSubsystem->GetPokeBallIcon(Pokemon->GetPokeBall()), true);
    PokemonStatusIcon->SetVisibility(Hidden);
}