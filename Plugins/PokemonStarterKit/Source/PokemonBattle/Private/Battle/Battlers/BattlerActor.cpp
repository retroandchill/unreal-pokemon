// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Battlers/BattlerActor.h"
#include "Algo/ForEach.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/BattlerSprite.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Mainpulation/RangeHelpers.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Stats/StatBlock.h"

#include <functional>

TScriptInterface<IBattleMove> CreateBattleMove(ABattlerActor* Battler, const TScriptInterface<IMove>& Move) {
    return NewObject<UBaseBattleMove>(Battler)->Initialize(Battler->GetOwningSide()->GetOwningBattle(), Move);
}


TScriptInterface<IBattler> ABattlerActor::Initialize(const TScriptInterface<IBattleSide> &Side, const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately) {
    OwningSide = Side;
    WrappedPokemon = Pokemon;
    InternalId = FGuid::NewGuid();
    auto MoveBlock = Pokemon->GetMoveBlock();
    Moves = RangeHelpers::CreateRange(MoveBlock->GetMoves())
        | std::views::transform(std::bind_front(&CreateBattleMove, this))
        | RangeHelpers::TToArray<TScriptInterface<IBattleMove>>();
    SpawnSpriteActor(ShowImmediately);
    return this;
}

FGuid ABattlerActor::GetInternalId() const {
    return InternalId;
}

TScriptInterface<IBattleSide> ABattlerActor::GetOwningSide() const {
    return OwningSide.ToScriptInterface();
}

FText ABattlerActor::GetNickname() const {
    return WrappedPokemon->GetNickname();
}

int32 ABattlerActor::GetPokemonLevel() const {
    return WrappedPokemon->GetStatBlock()->GetLevel();
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
    return WrappedPokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_ATTACK"))->GetStatValue();
}

TArray<FName> ABattlerActor::GetTypes() const {
    return WrappedPokemon->GetTypes();
}

bool ABattlerActor::IsAbilityActive() const {
    return true;
}

const TScriptInterface<IAbilityBattleEffect> & ABattlerActor::GetAbility() const {
    return Ability;
}

bool ABattlerActor::IsHoldItemActive() const {
    return true;
}

const TScriptInterface<IHoldItemBattleEffect> & ABattlerActor::GetHoldItem() const {
    return HoldItem;
}

const TArray<TScriptInterface<IBattleMove>> & ABattlerActor::GetMoves() const {
    return Moves;
}

void ABattlerActor::SelectActions() const {
}

uint8 ABattlerActor::GetActionCount() const {
    return 1;
}

void ABattlerActor::ForEachAlly(const TFunctionRef<void(const TScriptInterface<IBattler> &)> &Callback) const {
    Algo::ForEach(OwningSide->GetBattlers(), [this, &Callback](const TScriptInterface<IBattler>& Battler) {
        if (Battler->GetInternalId() == InternalId) {
            return;
        }

        Callback(Battler);
    });
}

void ABattlerActor::ForEachBattleEffect(const TFunctionRef<void(const TScriptInterface<IBattlerEffect> &)> &Callback) const {
}

void ABattlerActor::ShowSprite() const {
    check(Sprite != nullptr)
    Sprite->SetActorHiddenInGame(false);
}

void ABattlerActor::SpawnSpriteActor(bool ShouldShow) {
    Sprite = GetWorld()->SpawnActor<AActor>(BattlerSpriteClass.LoadSynchronous(), GetTransform());
    Sprite->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    IBattlerSprite::Execute_SetBattleSprite(Sprite, GraphicsSubsystem->GetPokemonBattleSprite(*WrappedPokemon, this, OwningSide->ShowBackSprites()));
    Sprite->SetActorHiddenInGame(!ShouldShow);
}