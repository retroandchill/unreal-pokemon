﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Components/ExpGainPane.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Blueprint/WidgetTree.h"
#include "Components/BattlerExpPanel.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "Utilities/TrainerHelpers.h"

void UExpGainPane::SetBattle(const TScriptInterface<IBattle> &Battle) {
    OwningBattle = Battle;

    check(PanelClass != nullptr)
    Algo::ForEach(Panels, &UWidget::RemoveFromParent);

    auto &PlayerTrainer = UTrainerHelpers::GetPlayerCharacter(this);
    // clang-format off
    Panels = OwningBattle->GetPlayerSide()->GetTrainerParty(PlayerTrainer) |
             Retro::Ranges::Views::Transform(Retro::BindMethod<&UExpGainPane::CreateBattlerPanel>(this)) |
             Retro::Ranges::To<TArray>();
    // clang-format on
}

void UExpGainPane::GainExp(TArray<FExpGainInfo> &&GainInfosIn) {
    GainInfos = std::move(GainInfosIn);
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
    Panels | Retro::Ranges::ForEach(Retro::BindBack<&UBattlerExpPanel::AnimateGain>(MaxDuration));
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