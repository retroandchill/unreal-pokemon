// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/ActiveSide.h"
#include "Algo/AnyOf.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/PlayerBattlerController.h"
#include "RangeHelpers.h"
#include "Pokemon/Pokemon.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"

TScriptInterface<IBattleSide> AActiveSide::Initialize(const TScriptInterface<IBattle> &Battle,
                                                      const TScriptInterface<IPokemon> &Pokemon, bool ShowBackSprites) {
    InternalId = FGuid::NewGuid();
    OwningBattle = Battle;
    SideSize = 1;
    bShowBackSprites = ShowBackSprites;
    Battlers.Reset();
    Trainers.Reset();
    TScriptInterface<IBattleSide> Side = this;
    auto Battler = GetWorld()->SpawnActor<AActor>(BattlerClass.LoadSynchronous(), GetBattlerSpawnPosition(0));
    Battlers.Emplace_GetRef(Battler)->Initialize(Side, Pokemon, true);
    Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    IntroMessageText = FText::FormatNamed(WildBattleTextFormat, TEXT("Pkmn"), Pokemon->GetNickname());
    SendOutText.Reset();
    return Side;
}

TScriptInterface<IBattleSide> AActiveSide::Initialize(const TScriptInterface<IBattle> &Battle,
                                                      const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount,
                                                      bool ShowBackSprites) {
    InternalId = FGuid::NewGuid();
    OwningBattle = Battle;
    SideSize = PokemonCount;
    bShowBackSprites = ShowBackSprites;
    Battlers.Reset();
    Trainers.Reset();
    Trainers.Emplace(Trainer);
    TScriptInterface<IBattleSide> Side = this;
    auto &Party = Trainer->GetParty();
    for (uint8 i = 0; i < PokemonCount; i++) {
        auto Battler = GetWorld()->SpawnActor<AActor>(BattlerClass.LoadSynchronous(), GetBattlerSpawnPosition(i));
        Battlers.Emplace_GetRef(Battler)->Initialize(Side, Party.IsValidIndex(i) ? Party[i] : nullptr, false);
        Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    }

    auto TrainerName = FText::FormatOrdered(FText::FromStringView(TEXT("{0} {1}")), Trainer->GetTrainerType().RealName,
                                            Trainer->GetTrainerName());
    IntroMessageText = FText::FormatNamed(WildBattleTextFormat, TEXT("Names"), TrainerName);

    // TODO: Add support for multiple battlers
    SendOutText = FText::FormatNamed(ShowBackSprites ? PlayerSendOutTextFormat : OpponentSendOutTextFormat,
                                     TEXT("Names"), TrainerName, TEXT("Pkmn"), Battlers[0]->GetNickname());
    return Side;
}

const FGuid &AActiveSide::GetInternalId() const {
    return InternalId;
}

const TScriptInterface<IBattle> &AActiveSide::GetOwningBattle() const {
    return OwningBattle;
}

uint8 AActiveSide::GetSideSize() const {
    return SideSize;
}

const FText &AActiveSide::GetIntroText() const {
    return IntroMessageText;
}

const TOptional<FText> &AActiveSide::GetSendOutText() const {
    return SendOutText;
}

bool AActiveSide::ShowBackSprites() const {
    return bShowBackSprites;
}

void AActiveSide::SendOutBattlers() const {
    Algo::ForEach(Battlers, [](const TScriptInterface<IBattler> &Battler) { Battler->ShowSprite(); });
}

const TArray<TScriptInterface<IBattler>> &AActiveSide::GetBattlers() const {
    return Battlers;
}

bool AActiveSide::CanBattle() const {
    return Algo::AnyOf(Battlers, [](const TScriptInterface<IBattler> &Battler) { return Battler->IsFainted(); });
}