// "Unreal Pokémon" created by Retro & Chill.


#include "Menus/PauseMenuBase.h"
#include "RangeHelpers.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Buttons/PokemonInfoButtonBase.h"
#include "Trainers/Trainer.h"
#include "Utilities/TrainerHelpers.h"
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/transform.hpp>

void UPauseMenuBase::NativeConstruct() {
    Super::NativeConstruct();

    auto Player = UTrainerHelpers::GetPlayerCharacter(this);
    auto &Party = Player->GetParty();
    Algo::ForEach(PokemonPanels, &UWidget::RemoveFromParent);
    PokemonPanels.Reset();
    PokemonPanels.Reserve(Party.Num());
    PokemonPanels = RangeHelpers::CreateRange(Player->GetParty())
        | ranges::views::transform(std::bind_front(&UPauseMenuBase::CreatePokemonPanel, this))
        | RangeHelpers::TToArray<TObjectPtr<UPokemonInfoButtonBase>>();
}

UPokemonInfoButtonBase * UPauseMenuBase::CreatePokemonPanel(const TScriptInterface<IPokemon> &Pokemon) {
    auto Panel = WidgetTree->ConstructWidget(PokemonPanelClass);
    Panel->SetPokemon(Pokemon, PokemonPanels.Num());
    SlotPokemonButton(Panel);
    return Panel;
}