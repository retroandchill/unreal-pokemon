// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/GainExpOnFaint.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/BattleScreenHelpers.h"

UGainExpOnFaint *UGainExpOnFaint::GainExpOnFaint(const UObject *WorldContextObject,
                                                 const TArray<TScriptInterface<IBattler>> &Battlers) {
    auto Node = NewObject<UGainExpOnFaint>();
    Node->WorldContextObject = WorldContextObject;
    Node->Battlers = Battlers;
    return Node;
}

void UGainExpOnFaint::Activate() {
    auto ValidBattlers = Battlers.FilterByPredicate([](const TScriptInterface<IBattler> &Battler) {
        auto &OwningSide = Battler->GetOwningSide();
        return OwningSide == OwningSide->GetOwningBattle()->GetOpposingSide();
    });
    if (ValidBattlers.IsEmpty()) {
        OnComplete.Broadcast();
        SetReadyToDestroy();
        return;
    }

    auto GainInfos = ValidBattlers[0]->GiveExpToParticipants();
    for (int32 i = 1; i < ValidBattlers.Num(); i++) {
        auto AdditionalInfos = ValidBattlers[i]->GiveExpToParticipants();
        check(GainInfos.Num() == AdditionalInfos.Num())

        for (int32 j = 0; j < GainInfos.Num(); j++) {
            auto &MainInfo = GainInfos[j];
            auto &CopiedInfo = AdditionalInfos[j];

            check(MainInfo.GainingBattler == CopiedInfo.GainingBattler)
            MainInfo.Amount += CopiedInfo.Amount;
            MainInfo.StatChanges += CopiedInfo.StatChanges;
        }
    }

    for (auto &Info : GainInfos) {
        if (Info.StatChanges.LevelChange.Diff() > 0) {
            Info.GainingBattler->RefreshStats();
        }
    }

    auto Screen = UBattleScreenHelpers::FindBattleScreen(WorldContextObject);
    if (Screen == nullptr) {
        OnComplete.Broadcast();
        SetReadyToDestroy();
        return;
    }

    CompleteHandle =
        Screen->BindToExpGainComplete(FSimpleDelegate::CreateUObject(this, &UGainExpOnFaint::OnExpGainComplete));
    Screen->DisplayExpForGain(std::move(GainInfos));
}

void UGainExpOnFaint::OnExpGainComplete() {
    auto Screen = UBattleScreenHelpers::FindBattleScreen(WorldContextObject);
    check(Screen != nullptr);
    Screen->FinishExpGainDisplay();
    OnComplete.Broadcast();
    Screen->RemoveFromExpGainComplete(CompleteHandle);
    SetReadyToDestroy();
}