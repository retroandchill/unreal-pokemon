// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/BattleScreenHelpers.h"
#include "PokemonBattleUI.h"
#include "PrimaryGameLayout.h"
#include "Battle/Battlers/Battler.h"
#include "Screens/PokemonBattleScreen.h"

UPokemonBattleScreen * UBattleScreenHelpers::FindBattleScreen(const UObject *WorldContextObject) {
    auto PrimaryLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    if (PrimaryLayout == nullptr) {
        UE_LOG(LogBattleUI, Warning, TEXT("No layout found, can't update battle HUD."))
        return nullptr;
    }

    auto ActiveWidget = PrimaryLayout->GetLayerWidget(RPG::Menus::PrimaryMenuLayerTag)->GetActiveWidget();
    return Cast<UPokemonBattleScreen>(ActiveWidget);
}

UPokemonBattlePanel * UBattleScreenHelpers::FindPokemonBattlePanel(const UObject *WorldContextObject,
    const TScriptInterface<IBattler> &Battler) {
    auto Screen = FindBattleScreen(WorldContextObject);
    if (Screen == nullptr) {
        UE_LOG(LogBattleUI, Warning, TEXT("No battle screen found, can't update."))
        return nullptr;
    }
    
    auto Panel = Screen->FindPanelForBattler(Battler);
#if !NO_LOGGING
    if (Panel == nullptr) {
        UE_LOG(LogBattleUI, Warning, TEXT("No battle found for %s, can't be updated!"), *Battler->GetNickname().ToString())
    }
#endif
    return Panel;
}

void UBattleScreenHelpers::RefreshDisplayedStatusCondition(const UObject *WorldContextObject,
                                                           const TScriptInterface<IBattler> &Battler) {
    auto Panel = FindPokemonBattlePanel(WorldContextObject, Battler);
    if (Panel == nullptr) {
        return;
    }

    Panel->RefreshStatusEffect();
}