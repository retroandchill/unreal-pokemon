// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Animations/UpdateHPBar.h"
#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Components/Party/HPBarPanel.h"
#include "Components/Party/PokemonPanel.h"
#include "Components/ProgressBar.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PartyDisplayScreen.h"
#include "Screens/Screen.h"
#include "Utilities/PokemonUIUtils.h"

UUpdateHPBar *UUpdateHPBar::UpdateHPBar(UScreen *Screen, const TScriptInterface<IPokemon> &Pokemon, float MaxDuration) {
    auto Node = NewObject<UUpdateHPBar>();
    Node->Screen = Screen;
    Node->Pokemon = Pokemon;
    Node->MaxDuration = MaxDuration;
    return Node;
}

void UUpdateHPBar::Activate() {
    using FSetPercent = Pokemon::UI::FSetNewPercent::FDelegate;
    if (!Screen->Implements<UPartyDisplayScreen>()) {
        OnAnimationComplete.Broadcast();
        SetReadyToDestroy();
        return;
    }

    auto SelectionPane = IPartyDisplayScreen::Execute_GetPokemonSelectionPane(Screen);
    check(IsValid(SelectionPane))

    auto Panel = SelectionPane->FindPanelForPokemon(Pokemon).GetPtrOrNull();
    if (!IsValid(Panel) || !Panel->Implements<UHPBarPanel>()) {
        OnAnimationComplete.Broadcast();
        SetReadyToDestroy();
        return;
    }

    auto ProgressBar = IHPBarPanel::Execute_GetHPBar(Panel);
    check(IsValid(ProgressBar))

    Animation.BindActionToPercentDelegate(FSetPercent::CreateWeakLambda(Panel, [this, Panel](float Percent) {
        float HPValue = FMath::RoundToFloat(Pokemon->GetMaxHP() * Percent);
        IHPBarPanel::Execute_UpdateHPBarPercent(Panel, Percent, HPValue);
    }));
    Animation.BindActionToCompleteDelegate(FSimpleDelegate::CreateWeakLambda(this, [this, Panel, ProgressBar] {
        IHPBarPanel::Execute_UpdateHPBarPercent(Panel, ProgressBar->GetPercent(), Pokemon->GetCurrentHP());
        OnAnimationComplete.Broadcast();
        SetReadyToDestroy();
    }));

    float HPPercent = ProgressBar->GetPercent();
    float OldHP = FMath::RoundToFloat(Pokemon->GetMaxHP() * HPPercent);
    float DrainRate =
        FMath::Min(FMath::Abs(OldHP - Pokemon->GetCurrentHP()) * UPokemonUIUtils::AnimationDrainSpeed, MaxDuration);
    float CurrentPercent = static_cast<float>(Pokemon->GetCurrentHP()) / static_cast<float>(Pokemon->GetMaxHP());
    Animation.PlayAnimation(HPPercent, CurrentPercent, DrainRate);
}