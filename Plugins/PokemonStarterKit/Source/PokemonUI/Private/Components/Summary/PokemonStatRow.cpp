// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/PokemonStatRow.h"
#include "CommonNumericTextBlock.h"
#include "CommonRichTextBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Primatives/DisplayText.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

void UPokemonStatRow::SetDisplayedStat(FName NewDisplayedStat) {
    DisplayedStat = NewDisplayedStat;
}

void UPokemonStatRow::Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::Refresh_Implementation(Pokemon);

    auto StatBlock = Pokemon->GetStatBlock();
    auto StatInfo = StatBlock->GetStat(DisplayedStat);
    auto Stat = StatInfo->GetStat();
    const auto &StatChanges = StatBlock->GetNature().StatChanges;

    auto StatMatcher = [this](const FNatureStatChange &Change) { return Change.Stat == DisplayedStat; };
    StatLabel->SetText(Stat.RealNameBrief);
    if (auto StatChange = StatChanges.FindByPredicate(StatMatcher); StatChange == nullptr || StatChange->Change == 0) {
        StatLabel->SetTextStyle(DefaultStyle);
    } else if (StatChange->Change > 0) {
        StatLabel->SetTextStyle(BoostedStyle);
    } else { // StatChange->Change < 0
        StatLabel->SetTextStyle(DecreasedStyle);
    }
    SetStatValueText(Pokemon, StatInfo);
}

UDisplayText *UPokemonStatRow::GetStatValue() const {
    return StatValue;
}

void UPokemonStatRow::SetStatValueText_Implementation(const TScriptInterface<IPokemon> &Pokemon,
                                                      const TScriptInterface<IStatEntry> &StatInfo) {
    StatValue->SetText(FText::FromString(FString::FromInt(StatInfo->GetStatValue())));
}