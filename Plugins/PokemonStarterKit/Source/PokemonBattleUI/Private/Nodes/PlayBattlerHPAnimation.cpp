// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/PlayBattlerHPAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/BattleScreenHelpers.h"

UPlayBattlerHPAnimation *UPlayBattlerHPAnimation::PlayBattlerHPAnimation(const UObject *WorldContextObject,
                                                                         const TScriptInterface<IBattler> &Battler,
                                                                         float MaxDuration) {
    auto Node = NewObject<UPlayBattlerHPAnimation>();
    Node->SetWorldContext(WorldContextObject);
    Node->Battler = Battler;
    Node->MaxDuration = MaxDuration;
    return Node;
}

UE5Coro::TCoroutine<> UPlayBattlerHPAnimation::ExecuteCoroutine(FForceLatentCoroutine ForceLatentCoroutine) {
    auto Panel = UBattleScreenHelpers::FindPokemonBattlePanel(GetWorldContext(), Battler);
    if (Panel == nullptr) {
        OnSelected.Broadcast();
        co_return;
    }

    co_await Panel->AnimateHP(MaxDuration);
    OnSelected.Broadcast();
}