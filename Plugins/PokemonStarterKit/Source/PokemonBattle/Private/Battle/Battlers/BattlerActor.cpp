// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerActor.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Battlers/BattlerController.h"
#include "Battle/Battlers/BattlerSprite.h"
#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/BattleSide.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "range/v3/view/filter.hpp"
#include "RangeHelpers.h"
#include <functional>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

TScriptInterface<IBattleMove> CreateBattleMove(ABattlerActor *Battler, const TScriptInterface<IMove> &Move) {
    check(Battler != nullptr)
    check(Battler->GetOwningSide() != nullptr)
    check(Move != nullptr)
    return NewObject<UBaseBattleMove>(Battler)->Initialize(Battler->GetOwningSide()->GetOwningBattle(), Move);
}

TScriptInterface<IBattler> ABattlerActor::Initialize(const TScriptInterface<IBattleSide> &Side,
                                                     const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately) {
    OwningSide = Side;
    WrappedPokemon = Pokemon;
    InternalId = FGuid::NewGuid();
    auto MoveBlock = Pokemon->GetMoveBlock();
    Moves = RangeHelpers::CreateRange(MoveBlock->GetMoves()) |
            std::views::transform(std::bind_front(&CreateBattleMove, this)) |
            RangeHelpers::TToArray<TScriptInterface<IBattleMove>>();
    SpawnSpriteActor(ShowImmediately);

    auto &Battle = OwningSide->GetOwningBattle();
    if (OwningSide->ShowBackSprites()) {
        Controller = NewObject<UPlayerBattlerController>(this)->SetBattle(Battle);
    } else {
        Controller = NewObject<UAIBattlerController>(this);
    }
    Controller->BindOnActionReady(
        FActionReady::CreateLambda(std::bind_front(&IBattle::QueueAction, Battle.GetInterface())));

    Ability.SetID(Pokemon->GetAbility()->GetAbilityID());

    return this;
}

FGuid ABattlerActor::GetInternalId() const {
    return InternalId;
}

const TScriptInterface<IBattleSide> &ABattlerActor::GetOwningSide() const {
    return OwningSide;
}

FText ABattlerActor::GetNickname() const {
    return WrappedPokemon->GetNickname();
}

EPokemonGender ABattlerActor::GetGender() const {
    return WrappedPokemon->GetGender();
}

int32 ABattlerActor::GetPokemonLevel() const {
    return WrappedPokemon->GetStatBlock()->GetLevel();
}

int32 ABattlerActor::GetHP() const {
    return WrappedPokemon->GetCurrentHP();
}

int32 ABattlerActor::GetMaxHP() const {
    return WrappedPokemon->GetMaxHP();
}

float ABattlerActor::GetHPPercent() const {
    return static_cast<float>(WrappedPokemon->GetCurrentHP()) / static_cast<float>(WrappedPokemon->GetMaxHP());
}

void ABattlerActor::TakeBattleDamage(int32 Damage) {
    WrappedPokemon->SetCurrentHP(WrappedPokemon->GetCurrentHP() - Damage);
}

bool ABattlerActor::IsFainted() const {
    return WrappedPokemon->IsFainted();
}

void ABattlerActor::Faint() const {
    IBattlerSprite::Execute_Faint(Sprite);
}

int32 ABattlerActor::GetAttack() const {
    // TODO: Don't use hard-coded string literals for these
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("ATTACK"))->GetStatValue();
}

int32 ABattlerActor::GetDefense() const {
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("DEFENSE"))->GetStatValue();
}

int32 ABattlerActor::GetSpecialAttack() const {
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_ATTACK"))->GetStatValue();
}

int32 ABattlerActor::GetSpecialDefense() const {
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_DEFENSE"))->GetStatValue();
}

int32 ABattlerActor::GetSpeed() const {
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("SPEED"))->GetStatValue();
}

float ABattlerActor::GetExpPercent() const {
    return WrappedPokemon->GetStatBlock()->GetExpPercent();
}

TArray<FName> ABattlerActor::GetTypes() const {
    return WrappedPokemon->GetTypes();
}

bool ABattlerActor::IsAbilityActive() const {
    return true;
}

UAbilityBattleEffect *ABattlerActor::GetAbility() const {
    return Ability.Get();
}

bool ABattlerActor::IsHoldItemActive() const {
    return true;
}

const TScriptInterface<IHoldItemBattleEffect> &ABattlerActor::GetHoldItem() const {
    return HoldItem;
}

const TArray<TScriptInterface<IBattleMove>> &ABattlerActor::GetMoves() const {
    return Moves;
}

void ABattlerActor::SelectActions() {
    Controller->InitiateActionSelection(this);
}

uint8 ABattlerActor::GetActionCount() const {
    return 1;
}

void ABattlerActor::ForEachAlly(TInterfaceCallback<IBattler> Callback) const {
    Algo::ForEach(OwningSide->GetBattlers(), [this, &Callback](const TScriptInterface<IBattler> &Battler) {
        if (Battler->GetInternalId() == InternalId) {
            return;
        }

        Callback(Battler);
    });
}

void ABattlerActor::ForEachBattleEffect(TInterfaceCallback<IBattlerEffect> Callback) const {
    // TODO: Not implemented yet, probably going to remove this and replace it with the GAS
}

void ABattlerActor::ForEachIndividualTraitHolder(TInterfaceCallback<IIndividualTraitHolder> Callback) const {
    if (auto AbilityEffect = Ability.Get(); AbilityEffect != nullptr) {
        Callback(AbilityEffect);
    }
}

bool ABattlerActor::ForAnyIndividualTraitHolder(
    const TFunctionRef<bool(const IIndividualTraitHolder &)> Predicate) const {
    if (auto AbilityEffect = Ability.Get(); AbilityEffect != nullptr && Predicate(*AbilityEffect)) {
        return true;
    }

    return false;
}

ranges::any_view<IIndividualTraitHolder *> ABattlerActor::GetTraitHolders() const {
    auto AbilityRange = ranges::views::single(Ability.Get());
    return AbilityRange |
           ranges::views::filter([](const IIndividualTraitHolder *TraitHolder) { return TraitHolder != nullptr; });
}

void ABattlerActor::ShowSprite() const {
    check(Sprite != nullptr)
    Sprite->SetActorHiddenInGame(false);
}

void ABattlerActor::SpawnSpriteActor(bool ShouldShow) {
    Sprite = GetWorld()->SpawnActor<AActor>(BattlerSpriteClass.LoadSynchronous(), GetTransform());
    Sprite->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    IBattlerSprite::Execute_SetBattleSprite(
        Sprite, GraphicsSubsystem->GetPokemonBattleSprite(*WrappedPokemon, this, OwningSide->ShowBackSprites()));
    Sprite->SetActorHiddenInGame(!ShouldShow);
}