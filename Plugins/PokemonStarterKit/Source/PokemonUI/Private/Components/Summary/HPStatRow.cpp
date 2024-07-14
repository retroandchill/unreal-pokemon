// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/HPStatRow.h"
#include "CommonNumericTextBlock.h"
#include "Pokemon/Stats/StatEntry.h"
#include "Utilities/PokemonUIUtils.h"

void UHPStatRow::SetStatValueText_Implementation(const TScriptInterface<IPokemon> &Pokemon,
                                                 const TScriptInterface<IStatEntry> &StatInfo) {
    GetStatValue()->SetText(
        FText::FromString(FString::Format(TEXT("{0}/{1}"), {Pokemon->GetCurrentHP(), StatInfo->GetStatValue()})));

    UPokemonUIUtils::SetBarValues(HPBar, static_cast<float>(Pokemon->GetCurrentHP()),
                                  static_cast<float>(Pokemon->GetMaxHP()));
}