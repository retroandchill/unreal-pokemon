// "Unreal Pokémon" created by Retro & Chill.


#include "Components/ExpGainPane.h"
#include "RangeHelpers.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattlerExpPanel.h"
#include <range/v3/view/transform.hpp>

void UExpGainPane::SetBattle(const TScriptInterface<IBattle> &Battle) {
    OwningBattle = Battle;
    
    check(PanelClass != nullptr)
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);
    Panels = RangeHelpers::CreateRange(OwningBattle->GetPlayerSide()->GetBattlers())
        | ranges::views::transform(std::bind_front(&UExpGainPane::CreateBattlerPanel, this))
        | RangeHelpers::TToArray<TObjectPtr<UBattlerExpPanel>>();
}

UBattlerExpPanel * UExpGainPane::CreateBattlerPanel(const TScriptInterface<IBattler> &Battler) {
    auto Panel = WidgetTree->ConstructWidget(PanelClass);
    Panel->SetBattler(Battler);
    SlotBattlerPanel(Panel);
    return Panel;
}