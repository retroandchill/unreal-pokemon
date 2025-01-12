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
    Node->SetWorldContext(WorldContextObject);
    Node->Battlers = Battlers;
    return Node;
}

UE5Coro::TCoroutine<> UGainExpOnFaint::ExecuteCoroutine(FForceLatentCoroutine) {
    co_await ProcessExpGain();
    OnComplete.Broadcast();
}

UE5Coro::TCoroutine<> UGainExpOnFaint::ProcessExpGain() const {
    auto ValidBattlers = Battlers.FilterByPredicate([](const TScriptInterface<IBattler> &Battler) {
        auto &OwningSide = Battler->GetOwningSide();
        return OwningSide == OwningSide->GetOwningBattle()->GetOpposingSide();
    });
    if (ValidBattlers.IsEmpty()) {
        co_return;
    }

    auto GainInfos = co_await ValidBattlers[0]->GiveExpToParticipants();
    for (int32 i = 1; i < ValidBattlers.Num(); i++) {
        auto AdditionalInfos = co_await ValidBattlers[i]->GiveExpToParticipants();
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

    auto Screen = UBattleScreenHelpers::FindBattleScreen(GetWorldContext());
    if (Screen == nullptr) {
        co_return;
    }

    co_await Screen->DisplayExpForGain(std::move(GainInfos));
    Screen->FinishExpGainDisplay();
}