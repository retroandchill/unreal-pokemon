// "Unreal Pokémon" created by Retro & Chill.

#include "Components/ExpGainPane.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattlerExpPanel.h"
#include "Managers/PokemonSubsystem.h"
#include "RangeHelpers.h"
#include "Utilities/TrainerHelpers.h"
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/view/transform.hpp>

void UExpGainPane::SetBattle(const TScriptInterface<IBattle> &Battle) {
    OwningBattle = Battle;

    check(PanelClass != nullptr)
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);

    auto &PlayerTrainer = UTrainerHelpers::GetPlayerCharacter(this);
    Panels = RangeHelpers::CreateRange(OwningBattle->GetPlayerSide()->GetTrainerParty(PlayerTrainer)) |
             ranges::views::transform(std::bind_front(&UExpGainPane::CreateBattlerPanel, this)) |
             RangeHelpers::TToArray<TObjectPtr<UBattlerExpPanel>>();
}

void UExpGainPane::GainExp(TArray<FExpGainInfo> &&GainInfosIn) {
    GainInfos = MoveTemp(GainInfosIn);
    check(Panels.Num() == GainInfos.Num())
    for (int32 i = 0; i < Panels.Num(); i++) {
        const auto &Panel = Panels[i];
        auto &GainInfo = GainInfos[i];
        Panel->SetBattler(GainInfo.GainingBattler, GainInfo.StatChanges.LevelChange.Before,
                          GainInfo.StatChanges.ExpPercentChange.Before);
        Panel->ChangeExpGainDisplay(GainInfo.Amount);
    }
}

void UExpGainPane::PlayExpGain(float MaxDuration) {
    AnimationsComplete = 0;
    Algo::ForEach(Panels, ranges::bind_back(&UBattlerExpPanel::AnimateGain, MaxDuration));
}

const TArray<FExpGainInfo> &UExpGainPane::GetGainInfos() const {
    return GainInfos;
}

UBattlerExpPanel *UExpGainPane::CreateBattlerPanel(const TScriptInterface<IBattler> &Battler) {
    auto Panel = WidgetTree->ConstructWidget(PanelClass);
    Panel->SetBattler(Battler);
    SlotBattlerPanel(Panel);
    Panel->BindOnAnimationComplete(FSimpleDelegate::CreateWeakLambda(this, [this] {
        AnimationsComplete++;
        if (AnimationsComplete == Panels.Num()) {
            OnExpGainComplete.Broadcast();
        }
    }));
    return Panel;
}