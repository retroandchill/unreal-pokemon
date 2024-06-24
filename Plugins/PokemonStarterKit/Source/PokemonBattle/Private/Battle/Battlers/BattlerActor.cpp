// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerActor.h"
#include "DataManager.h"
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
#include "Battle/Abilities/AbilityLookup.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/Moves/MoveLookup.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "Species/PokemonStatType.h"
#include "Species/Stat.h"
#include <functional>
#include <range/v3/view/empty.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

TScriptInterface<IBattleMove> CreateBattleMove(ABattlerActor *Battler, const TScriptInterface<IMove> &Move) {
    check(Battler != nullptr)
    check(Battler->GetOwningSide() != nullptr)
    check(Move != nullptr)
    auto MoveClass = Battle::Moves::LookupMoveEffectClass(Move->GetFunctionCode());
    TScriptInterface<IBattleMove> BattleMove = NewObject<UObject>(Battler, MoveClass);
    BattleMove->Initialize(Battler->GetOwningSide()->GetOwningBattle(), Move);
    return BattleMove;
}

ABattlerActor::ABattlerActor() {
    BattlerAbilityComponent = CreateDefaultSubobject<UBattlerAbilityComponent>("BattlerAbilityComponent");
}

TScriptInterface<IBattler> ABattlerActor::Initialize(const TScriptInterface<IBattleSide> &Side,
                                                     const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately) {
    OwningSide = Side;
    WrappedPokemon = Pokemon;
    InternalId = FGuid::NewGuid();

    auto& DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();

    StatStages.Reset();
    StatTable.ForEach([this](const FStat &Stat) {
        if (Stat.Type == EPokemonStatType::Main)
            return;

        StatStages.Emplace(Stat.ID, 0);
    });
    
    auto MoveBlock = Pokemon->GetMoveBlock();
    Moves = RangeHelpers::CreateRange(MoveBlock->GetMoves()) |
            ranges::views::transform(std::bind_front(&CreateBattleMove, this)) |
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

    if (auto AbilityClass = Battle::Abilities::CreateAbilityEffect(Pokemon->GetAbility()->GetAbilityID(), this); AbilityClass != nullptr) {
        auto AbilityEffect = NewObject<UGameplayAbility>(this, AbilityClass);
        Ability = BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(AbilityEffect, 1, INDEX_NONE, this));
    } else {
        Ability = FGameplayAbilitySpecHandle();
    }

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

FMainBattleStat ABattlerActor::GetAttack() const {
    static const FName Attack = TEXT("ATTACK");
    return { WrappedPokemon->GetStatBlock()->GetStat(Attack)->GetStatValue(), GetStatStage(Attack) };
}

FMainBattleStat ABattlerActor::GetDefense() const {
    static const FName Defense = TEXT("DEFENSE");
    return { WrappedPokemon->GetStatBlock()->GetStat(Defense)->GetStatValue(), GetStatStage(Defense) };
}

FMainBattleStat ABattlerActor::GetSpecialAttack() const {
    static const FName SpecialAttack = TEXT("SPECIAL_ATTACK");
    return { WrappedPokemon->GetStatBlock()->GetStat(SpecialAttack)->GetStatValue(), GetStatStage(SpecialAttack) };
}

FMainBattleStat ABattlerActor::GetSpecialDefense() const {
    static const FName SpecialDefense = TEXT("SPECIAL_DEFENSE");
    return { WrappedPokemon->GetStatBlock()->GetStat(SpecialDefense)->GetStatValue(), GetStatStage(SpecialDefense) };
}

FMainBattleStat ABattlerActor::GetSpeed() const {
    static const FName Speed = TEXT("SPEED");
    return { WrappedPokemon->GetStatBlock()->GetStat(Speed)->GetStatValue(), GetStatStage(Speed) };
}

int32 ABattlerActor::GetStatStage(FName Stat) const {
    check(StatStages.Contains(Stat))
    return StatStages[Stat];
}

float ABattlerActor::GetExpPercent() const {
    return WrappedPokemon->GetStatBlock()->GetExpPercent();
}

TArray<FName> ABattlerActor::GetTypes() const {
    return WrappedPokemon->GetTypes();
}

UBattlerAbilityComponent * ABattlerActor::GetAbilityComponent() const {
    return BattlerAbilityComponent;
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

ranges::any_view<TScriptInterface<IBattler>> ABattlerActor::GetAllies() const {
    return RangeHelpers::CreateRange(OwningSide->GetBattlers()) |
           ranges::views::filter(
               [this](const TScriptInterface<IBattler> &Battler) { return Battler->GetInternalId() == InternalId; });
}

ranges::any_view<IIndividualTraitHolder *> ABattlerActor::GetTraitHolders() const {
    return ranges::views::empty<IIndividualTraitHolder *>;
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