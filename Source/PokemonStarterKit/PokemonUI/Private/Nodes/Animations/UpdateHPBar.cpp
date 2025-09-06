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

UUpdateHPBar *UUpdateHPBar::UpdateHPBar(UScreen *Screen, const TScriptInterface<IPokemon> &Pokemon, float MaxDuration)
{
    auto Node = NewObject<UUpdateHPBar>();
    Node->SetWorldContext(Screen);
    Node->Screen = Screen;
    Node->Pokemon = Pokemon;
    Node->MaxDuration = MaxDuration;
    return Node;
}

UE5Coro::TCoroutine<> UUpdateHPBar::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine)
{
    using Pokemon::UI::FSetNewPercent;
    if (!Screen->Implements<UPartyDisplayScreen>())
    {
        OnAnimationComplete.Broadcast();
        co_return;
    }

    auto SelectionPane = IPartyDisplayScreen::Execute_GetPokemonSelectionPane(Screen);
    check(IsValid(SelectionPane))

    auto Panel = SelectionPane->FindPanelForPokemon(Pokemon).GetPtrOrNull();
    if (!IsValid(Panel) || !Panel->Implements<UHPBarPanel>())
    {
        OnAnimationComplete.Broadcast();
        co_return;
    }

    auto ProgressBar = IHPBarPanel::Execute_GetHPBar(Panel);
    check(IsValid(ProgressBar))

    float HPPercent = ProgressBar->GetPercent();
    float OldHP = FMath::RoundToFloat(Pokemon->GetMaxHP() * HPPercent);
    float DrainRate =
        FMath::Min(FMath::Abs(OldHP - Pokemon->GetCurrentHP()) * UPokemonUIUtils::AnimationDrainSpeed, MaxDuration);
    float CurrentPercent = static_cast<float>(Pokemon->GetCurrentHP()) / static_cast<float>(Pokemon->GetMaxHP());
    co_await Pokemon::UI::ProgressBarAnimation(GetWorldContext(), HPPercent, CurrentPercent, DrainRate,
                                               FSetNewPercent::CreateWeakLambda(Panel, [this, Panel](float Percent) {
                                                   float HPValue = FMath::RoundToFloat(Pokemon->GetMaxHP() * Percent);
                                                   IHPBarPanel::Execute_UpdateHPBarPercent(Panel, Percent, HPValue);
                                               }));

    IHPBarPanel::Execute_UpdateHPBarPercent(Panel, ProgressBar->GetPercent(), Pokemon->GetCurrentHP());
    OnAnimationComplete.Broadcast();
}