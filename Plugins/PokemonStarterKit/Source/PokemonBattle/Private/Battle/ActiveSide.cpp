// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/ActiveSide.h"
#include "Battle/Battlers/Battler.h"
#include "Trainers/Trainer.h"
#include "Battle/Battle.h"


TScriptInterface<IBattleSide> AActiveSide::Initialize(const TScriptInterface<IBattle> &Battle,
                                                      const TScriptInterface<IPokemon> &Pokemon, bool ShowBackSprites) {
    OwningBattle = Battle;
    SideSize = 1;
    bShowBackSprites = ShowBackSprites;
    Battlers.Reset();
    TScriptInterface<IBattleSide> Side = this;
    auto Battler = GetWorld()->SpawnActor<AActor>(BattlerClass.LoadSynchronous(), GetBattlerSpawnPosition(0));
    Battlers.Emplace_GetRef(Battler)->Initialize(Side, Pokemon, true);
    Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    return Side;
}

TScriptInterface<IBattleSide> AActiveSide::Initialize(const TScriptInterface<IBattle> &Battle,
                                                      const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount, bool ShowBackSprites) {
    OwningBattle = Battle;
    SideSize = PokemonCount;
    bShowBackSprites = ShowBackSprites;
    Battlers.Reset();
    TScriptInterface<IBattleSide> Side = this;
    auto &Party = Trainer->GetParty();
    for (uint8 i = 0; i < PokemonCount; i++) {
        auto Battler = GetWorld()->SpawnActor<AActor>(BattlerClass.LoadSynchronous(), GetBattlerSpawnPosition(i));
        Battlers.Emplace_GetRef(Battler)->Initialize(Side, Party.IsValidIndex(i) ? Party[i] : nullptr, false);
        Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    }
    return Side;
}

TScriptInterface<IBattle> AActiveSide::GetOwningBattle() const {
    return OwningBattle.ToScriptInterface();
}

uint8 AActiveSide::GetSideSize() const {
    return SideSize;
}

bool AActiveSide::ShowBackSprites() const {
    return bShowBackSprites;
}

const TArray<TScriptInterface<IBattler>> & AActiveSide::GetBattlers() const {
    return Battlers;
}