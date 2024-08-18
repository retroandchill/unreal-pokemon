// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/ActiveSide.h"
#include "Algo/AnyOf.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/BattleSideAbilitySystemComponent.h"
#include "Pokemon/Pokemon.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/ContainerView.h"
#include "Strings/StringUtilities.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

static void SwapForNonFaintedBattler(uint8 Start, TArray<TScriptInterface<IBattler>> &BattleParty) {
    for (uint8 j = Start + 1; j < BattleParty.Num(); j++) {
        if (auto &Substitute = BattleParty[j]; !Substitute->IsFainted()) {
            BattleParty.Swap(Start, j);
        }
    }
}

AActiveSide::AActiveSide() {
    AbilitySystemComponent = CreateDefaultSubobject<UBattleSideAbilitySystemComponent>(FName("AbilitySystemComponent"));
}

TScriptInterface<IBattleSide> AActiveSide::Initialize(const TScriptInterface<IBattle> &Battle,
                                                      const TArray<TScriptInterface<IPokemon>> &Pokemon,
                                                      bool ShowBackSprites) {
    InternalId = FGuid::NewGuid();
    OwningBattle = Battle;
    SideSize = 1;
    bShowBackSprites = ShowBackSprites;
    Battlers.Reset();
    Trainers.Reset();
    TScriptInterface<IBattleSide> Side = this;

    auto BattlerActorClass = BattlerClass.LoadSynchronous();
    TArray<FText> Nicknames;
    for (auto &Pkmn : Pokemon) {
        auto Battler = GetWorld()->SpawnActor<AActor>(BattlerActorClass, GetBattlerSpawnPosition(0));
        Battlers.Emplace_GetRef(Battler)->Initialize(Side, Pkmn, true);
        Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
        Nicknames.Add(Pkmn->GetNickname());
    }

    static auto Contraction = NSLOCTEXT("PokemonBattle_ActiveSide", "IntroContraction", "and");
    auto FullOpponentName = UStringUtilities::GenerateList(Nicknames, Contraction);
    IntroMessageText = FText::FormatNamed(WildBattleTextFormat, TEXT("Pkmn"), FullOpponentName);
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
    auto &BattleParty = TrainerParties.Add(Trainer->GetInternalId()).Battlers;
    for (uint8 i = 0; i < Party.Num(); i++) {
        auto SpawnPosition = i < PokemonCount ? GetBattlerSpawnPosition(i) : GetTransform();
        auto Battler = GetWorld()->SpawnActor<AActor>(BattlerClass.LoadSynchronous(), SpawnPosition);
        BattleParty.Emplace_GetRef(Battler)->Initialize(Side, Party.IsValidIndex(i) ? Party[i] : nullptr, false);
        Battler->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    }

    for (uint8 i = 0; i < BattleParty.Num(); i++) {
        if (BattleParty[i]->IsFainted()) {
            SwapForNonFaintedBattler(i, BattleParty);
        }

        auto &Battler = BattleParty[i];
        check(!Battler->IsFainted())
        if (i < PokemonCount) {
            Battlers.Emplace_GetRef(Battler);
        }
    }

    auto TrainerName = FText::FormatOrdered(FText::FromStringView(TEXT("{0} {1}")), Trainer->GetTrainerType().RealName,
                                            Trainer->GetTrainerName());
    IntroMessageText = FText::FormatNamed(TrainerBattleTextFormat, TEXT("Names"), TrainerName);

    // TODO: Add support for multiple battlers
    SendOutText = FText::FormatNamed(ShowBackSprites ? PlayerSendOutTextFormat : OpponentSendOutTextFormat,
                                     TEXT("Names"), TrainerName, TEXT("Pkmn"), Battlers[0]->GetNickname());
    return Side;
}

void AActiveSide::BeginPlay() {
    Super::BeginPlay();
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AActiveSide::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    Battlers |
        ranges::views::transform(&UE::Ranges::CastInterfaceChecked<AActor>) |
        UE::Ranges::ForEach([](AActor* A) { return A->Destroy(); });
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
    Algo::ForEach(Battlers, &IBattler::ShowSprite);
}

const TArray<TScriptInterface<IBattler>> &AActiveSide::GetBattlers() const {
    return Battlers;
}

const TArray<TScriptInterface<ITrainer>> &AActiveSide::GetTrainers() const {
    return Trainers;
}

const TArray<TScriptInterface<IBattler>> &
AActiveSide::GetTrainerParty(const TScriptInterface<ITrainer> &Trainer) const {
    return TrainerParties.FindChecked(Trainer->GetInternalId()).Battlers;
}

void AActiveSide::SwapBattlerPositions(const TScriptInterface<ITrainer> &Trainer, int32 IndexA, int32 IndexB) {
    auto &BattlerArray = TrainerParties.FindChecked(Trainer->GetInternalId()).Battlers;
    BattlerArray.Swap(IndexA, IndexB);

    if (auto BattlePartyIndex = Battlers.Find(BattlerArray[IndexB]); BattlePartyIndex != INDEX_NONE) {
        Battlers[BattlePartyIndex] = BattlerArray[IndexA];
    }
}

bool AActiveSide::CanBattle() const {
    if (Algo::AnyOf(Battlers, &IBattler::IsNotFainted)) {
        return true;
    }

    for (auto &[ID, Party] : TrainerParties) {
        if (Algo::AnyOf(Party.Battlers, &IBattler::IsNotFainted)) {
            return true;
        }
    }

    return false;
}