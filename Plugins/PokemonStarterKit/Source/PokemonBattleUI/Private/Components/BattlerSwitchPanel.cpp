// "Unreal Pokémon" created by Retro & Chill.


#include "Components/BattlerSwitchPanel.h"

const TScriptInterface<IBattler> & UBattlerSwitchPanel::GetBattler() const {
    return CurrentBattler;
}

void UBattlerSwitchPanel::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
    Refresh();
}