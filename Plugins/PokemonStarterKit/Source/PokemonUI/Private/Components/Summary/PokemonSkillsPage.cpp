// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/PokemonSkillsPage.h"
#include "Blueprint/WidgetTree.h"
#include "CommonTextBlock.h"
#include "Components/DisplayText.h"
#include "Components/Summary/HPStatRow.h"
#include "Components/Summary/PokemonStatRow.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Pokemon.h"
#include "Settings/BaseSettings.h"

TSharedRef<SWidget> UPokemonSkillsPage::RebuildWidget() {
    auto Ret = Super::RebuildWidget();

    for (const auto &StatRow : StatRows) {
        StatRow->RemoveFromParent();
    }
    StatRows.Empty();

    if (HPStatRow != nullptr && StatRowClass != nullptr) {
        auto HPStat = Pokemon::FBaseSettings::Get().GetHPStat();
        for (auto Stat : StatsToDisplay) {
            if (Stat == HPStat) {
                auto Row = WidgetTree->ConstructWidget(HPStatRow);
                Row->SetDisplayedStat(Stat);
                SlotHPStatRow(Row);
                StatRows.Emplace(Row);
            } else {
                auto Row = WidgetTree->ConstructWidget(StatRowClass);
                Row->SetDisplayedStat(Stat);
                SlotStatRow(Row);
                StatRows.Emplace(Row);
            }
        }
    }

    return Ret;
}

void UPokemonSkillsPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::RefreshInfo_Implementation(Pokemon);
    for (const auto &Row : StatRows) {
        Row->Refresh(Pokemon);
    }

    auto Ability = Pokemon->GetAbility();
    AbilityName->SetText(Ability->GetDisplayName());
    AbilityDescription->SetText(Ability->GetAbilityDescription());
}