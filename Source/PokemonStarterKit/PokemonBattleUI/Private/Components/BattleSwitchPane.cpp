// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleSwitchPane.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattlerSwitchPanel.h"
#include "Utilities/TrainerHelpers.h"

void UBattleSwitchPane::SetBattle(const TScriptInterface<IBattle> &Battle)
{
    CurrentBattle = Battle;
    RefreshContent();
}

void UBattleSwitchPane::RefreshContent()
{
    ClearSelectableOptions();
    auto &PlayerTrainer = UTrainerHelpers::GetPlayerCharacter(this);
    check(CurrentBattle != nullptr)
    for (auto &PlayerParty = CurrentBattle->GetPlayerSide()->GetTrainerParty(PlayerTrainer);
         auto &Battler : PlayerParty)
    {
        auto Panel = WidgetTree->ConstructWidget(PanelClass);
        Panel->SetBattler(Battler);
        SlotOption(Panel);
    }
}

void UBattleSwitchPane::NativeOnSelectionChange(int32 OldIndex, int32 NewIndex)
{
    Super::NativeOnSelectionChange(OldIndex, NewIndex);
    auto Option = GetSelectableOption<UBattlerSwitchPanel>(NewIndex);
    if (NewIndex == INDEX_NONE)
    {
        return;
    }
    check(Option != nullptr)
    OnSelectedBattlerChanged.Broadcast(Option->GetBattler());
}