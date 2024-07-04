// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/PlayBattlerHPAnimation.h"
#include "RPGMenusSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/BattleScreenHelpers.h"

UPlayBattlerHPAnimation * UPlayBattlerHPAnimation::PlayBattlerHPAnimation(const UObject *WorldContextObject,
                                                                          const TScriptInterface<IBattler> &Battler, float MaxDuration) {
    auto Node = NewObject<UPlayBattlerHPAnimation>();
    Node->WorldContextObject = WorldContextObject;
    Node->Battler = Battler;
    Node->MaxDuration = MaxDuration;
    return Node;
}

void UPlayBattlerHPAnimation::Activate() {
    auto Panel = UBattleScreenHelpers::FindPokemonBattlePanel(WorldContextObject, Battler);
    if (Panel == nullptr) {
        ExecuteOnComplete();
        return;
    }

    FOnProgresBarUpdateComplete::FDelegate Callback;
    Callback.BindDynamic(this, &UPlayBattlerHPAnimation::ExecuteOnComplete);
    Panel->BindToOnProgressBarUpdateComplete(Callback);
    UnbindActions.BindWeakLambda(Panel, &UPokemonBattlePanel::UnbindAllHPUpdateDelegates, Panel, this);
    Panel->AnimateHP(MaxDuration);
}

void UPlayBattlerHPAnimation::ExecuteOnComplete() {
    OnSelected.Broadcast();
    UnbindActions.ExecuteIfBound();
}