// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PokemonActionOptions.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattleMenuOption.h"
#include "Mainpulation/RangeHelpers.h"

#include <functional>

TSharedRef<SWidget> UPokemonActionOptions::RebuildWidget() {
    auto Ret = Super::RebuildWidget();

    Algo::ForEach(Options, &UWidget::RemoveFromParent);
    Options.Reset();
    if (OptionClass != nullptr) {
        Options = RangeHelpers::CreateRange(MenuActions)
            | std::views::transform(std::bind_front(&UPokemonActionOptions::CreateMenuOption, this))
            | RangeHelpers::TToArray<TObjectPtr<UBattleMenuOption>>();
    }
    
    return Ret;
}

int32 UPokemonActionOptions::GetItemCount_Implementation() const {
    return MenuActions.Num();
}

TObjectPtr<UBattleMenuOption> UPokemonActionOptions::CreateMenuOption(const UBattleMenuHandler* MenuHandler) {
    TObjectPtr<UBattleMenuOption> Option = WidgetTree->ConstructWidget(OptionClass);
    Option->InitFromHandler(MenuHandler);
    SlotOption(Option);
    return Option;
}