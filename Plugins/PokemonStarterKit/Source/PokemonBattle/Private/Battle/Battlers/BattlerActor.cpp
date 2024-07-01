// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerActor.h"
#include "DataManager.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Battlers/BattlerController.h"
#include "Battle/Battlers/BattlerSprite.h"
#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/BattleSide.h"
#include "Battle/Moves/PokemonBattleMove.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "range/v3/view/filter.hpp"
#include "RangeHelpers.h"
#include "Battle/Abilities/AbilityLookup.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"
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
    TScriptInterface<IBattleMove> BattleMove = NewObject<UPokemonBattleMove>(Battler);
    BattleMove->Initialize(Battler, Move);
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

    
    TMap<UClass*, UAttributeSet*> Attributes;
    for (auto AttributeSets = BattlerAbilityComponent->GetSpawnedAttributes(); auto Attribute : AttributeSets) {
        Attributes.Add(Attribute->GetClass(), Attribute);
    }

    auto StatBlock = WrappedPokemon->GetStatBlock();
    StatTable.ForEach([this, &Attributes, &StatBlock](const FStat &Stat) {
        if (Stat.BaseAttribute.IsValid() && Stat.Type != EPokemonStatType::Battle) {
            auto AttributeSetClass = Stat.BaseAttribute.GetAttributeSetClass();
            check(Attributes.Contains(AttributeSetClass))
            auto AttributeData = Stat.BaseAttribute.GetGameplayAttributeDataChecked(Attributes[AttributeSetClass]);
            auto StatValue = StatBlock->GetStat(Stat.ID);
            AttributeData->SetBaseValue(static_cast<float>(StatValue->GetStatValue()));
            AttributeData->SetCurrentValue(AttributeData->GetBaseValue());
        }
        
        if (Stat.StagesAttribute.IsValid()) {
            auto AttributeSetClass = Stat.StagesAttribute.GetAttributeSetClass();
            check(Attributes.Contains(AttributeSetClass))
            auto AttributeData = Stat.StagesAttribute.GetGameplayAttributeDataChecked(Attributes[AttributeSetClass]);
            AttributeData->SetBaseValue(0.f);
            AttributeData->SetCurrentValue(0.f);
        }
    });

    BattlerAbilityComponent->GetCoreAttributes()->InitHP(static_cast<float>(WrappedPokemon->GetCurrentHP()));
    auto &HPChangedDelegate = BattlerAbilityComponent->GetGameplayAttributeValueChangeDelegate(UPokemonCoreAttributeSet::GetHPAttribute());
    HPChangedDelegate.Clear();
    HPChangedDelegate.AddUObject(this, &ABattlerActor::UpdateHPValue);
    
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

    if (auto AbilityClass = Battle::Abilities::CreateAbilityEffect(Pokemon->GetAbility()->GetAbilityID()); AbilityClass != nullptr) {
        Ability = BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
    } else {
        Ability = FGameplayAbilitySpecHandle();
    }

    return this;
}

void ABattlerActor::BeginPlay() {
    Super::BeginPlay();
    BattlerAbilityComponent->InitAbilityActorInfo(this, this);
    InnateAbilityHandles = RangeHelpers::CreateRange(InnateAbilities)
        | ranges::views::transform([this](TSubclassOf<UGameplayAbility> Type) {
            return BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(Type, 1, INDEX_NONE, this));
        })
        | RangeHelpers::TToArray<FGameplayAbilitySpecHandle>();
}

void ABattlerActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    BattlerAbilityComponent->ClearAbility(Ability);
    for (auto Innate : InnateAbilityHandles) {
        BattlerAbilityComponent->ClearAbility(Innate);
    }
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

float ABattlerActor::GetHPPercent() const {
    auto CoreAttributes = BattlerAbilityComponent->GetCoreAttributes();
    return CoreAttributes->GetHP() / CoreAttributes->GetMaxHP();
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

void ABattlerActor::ShowSprite() const {
    check(Sprite != nullptr)
    Sprite->SetActorHiddenInGame(false);
}

void ABattlerActor::UpdateHPValue(const FOnAttributeChangeData &Data) const {
    WrappedPokemon->SetCurrentHP(FMath::FloorToInt32(Data.NewValue));
}

void ABattlerActor::SpawnSpriteActor(bool ShouldShow) {
    Sprite = GetWorld()->SpawnActor<AActor>(BattlerSpriteClass.LoadSynchronous(), GetTransform());
    Sprite->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    IBattlerSprite::Execute_SetBattleSprite(
        Sprite, GraphicsSubsystem->GetPokemonBattleSprite(*WrappedPokemon, this, OwningSide->ShowBackSprites()));
    Sprite->SetActorHiddenInGame(!ShouldShow);
}