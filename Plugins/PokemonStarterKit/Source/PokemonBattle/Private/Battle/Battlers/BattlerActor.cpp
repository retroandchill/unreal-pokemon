// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerActor.h"
#include "Battle/Abilities/AbilityLookup.h"
#include "Battle/Attributes/ExpAttributeSet.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/AIBattlerController.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Battlers/BattlerController.h"
#include "Battle/Battlers/BattlerSprite.h"
#include "Battle/Battlers/Innate/Innate_CriticalHitDamage.h"
#include "Battle/Battlers/Innate/Innate_DamageSwing.h"
#include "Battle/Battlers/Innate/Innate_MultiTargetDamageSplit.h"
#include "Battle/Battlers/PlayerBattlerController.h"
#include "Battle/BattleSide.h"
#include "Battle/Items/ItemLookup.h"
#include "Battle/Moves/MoveLookup.h"
#include "Battle/Moves/PokemonBattleMove.h"
#include "DataManager.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Moves/MoveData.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/Move.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "PokemonBattleSettings.h"
#include "range/v3/view/filter.hpp"
#include "RangeHelpers.h"
#include "Species/PokemonStatType.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/TrainerHelpers.h"
#include <functional>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

TScriptInterface<IBattleMove> CreateBattleMove(ABattlerActor *Battler, const TScriptInterface<IMove> &Move) {
    check(Battler != nullptr)
    check(Move != nullptr)
    TScriptInterface<IBattleMove> BattleMove = NewObject<UPokemonBattleMove>(Battler);
    BattleMove->Initialize(Battler, Move);
    return BattleMove;
}

ABattlerActor::ABattlerActor() {
    BattlerAbilityComponent = CreateDefaultSubobject<UBattlerAbilityComponent>("BattlerAbilityComponent");
    InnateAbilities.Add(UInnate_CriticalHitDamage::StaticClass());
    InnateAbilities.Add(UInnate_MultiTargetDamageSplit::StaticClass());
    InnateAbilities.Add(UInnate_DamageSwing::StaticClass());
}

TScriptInterface<IBattler> ABattlerActor::Initialize(const TScriptInterface<IBattleSide> &Side,
                                                     const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately) {
    OwningSide = Side;
    WrappedPokemon = Pokemon;
    InternalId = FGuid::NewGuid();

    auto &DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();

    TMap<UClass *, UAttributeSet *> Attributes;
    for (auto AttributeSets = BattlerAbilityComponent->GetSpawnedAttributes(); auto Attribute : AttributeSets) {
        Attributes.Add(Attribute->GetClass(), Attribute);
    }

    auto StatBlock = WrappedPokemon->GetStatBlock();
    StatTable.ForEach([this, &Attributes, &StatBlock](const FStat &Stat) {
        if (Stat.BaseAttribute.IsValid() && Stat.Type != EPokemonStatType::Battle) {
            auto StatValue = StatBlock->GetStat(Stat.ID);
            BattlerAbilityComponent->SetNumericAttributeBase(Stat.BaseAttribute,
                                                             static_cast<float>(StatValue->GetStatValue()));
        }

        if (Stat.StagesAttribute.IsValid()) {
            BattlerAbilityComponent->SetNumericAttributeBase(Stat.StagesAttribute, 0.f);
        }
    });

    BattlerAbilityComponent->GetCoreAttributes()->InitHP(static_cast<float>(WrappedPokemon->GetCurrentHP()));
    auto &HPChangedDelegate =
        BattlerAbilityComponent->GetGameplayAttributeValueChangeDelegate(UPokemonCoreAttributeSet::GetHPAttribute());
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

    if (auto AbilityClass = Battle::Abilities::CreateAbilityEffect(Pokemon->GetAbility()->GetAbilityID());
        AbilityClass != nullptr) {
        Ability = BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
    } else {
        Ability = FGameplayAbilitySpecHandle();
    }

    if (auto HoldItemClass = Pokemon::Battle::Items::FindHoldItemEffect(Pokemon->GetHoldItem());
        HoldItemClass != nullptr) {
        HoldItem = BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(HoldItemClass, 1, INDEX_NONE, this));
    } else {
        HoldItem = FGameplayAbilitySpecHandle();
    }

    return this;
}

void ABattlerActor::BeginPlay() {
    Super::BeginPlay();
    BattlerAbilityComponent->InitAbilityActorInfo(this, this);
    InnateAbilityHandles =
        RangeHelpers::CreateRange(InnateAbilities) |
        ranges::views::transform([this](const TSubclassOf<UGameplayAbility> &Type) {
            return BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(Type, 1, INDEX_NONE, this));
        }) |
        RangeHelpers::TToArray<FGameplayAbilitySpecHandle>();
    InnateEffectHandles = RangeHelpers::CreateRange(InnateEffects) |
                          ranges::views::transform([this](const TSubclassOf<UGameplayEffect> &Effect) {
                              auto Context = BattlerAbilityComponent->MakeEffectContext();
                              auto SpecHandle = BattlerAbilityComponent->MakeOutgoingSpec(Effect, 1, Context);
                              return BattlerAbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
                          }) |
                          RangeHelpers::TToArray<FActiveGameplayEffectHandle>();
}

void ABattlerActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    BattlerAbilityComponent->ClearAbility(Ability);
    for (auto Innate : InnateAbilityHandles) {
        BattlerAbilityComponent->ClearAbility(Innate);
    }
    if (Sprite != nullptr) {
        Sprite->Destroy();
    }
}

FGuid ABattlerActor::GetInternalId() const {
    return InternalId;
}

const TScriptInterface<IBattleSide> &ABattlerActor::GetOwningSide() const {
    return OwningSide;
}

const TScriptInterface<IPokemon> &ABattlerActor::GetWrappedPokemon() const {
    return WrappedPokemon;
}

const FSpeciesData &ABattlerActor::GetSpecies() const {
    return WrappedPokemon->GetSpecies();
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

void ABattlerActor::RefreshStats() {
    auto &DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();
    auto StatBlock = WrappedPokemon->GetStatBlock();
    StatTable.ForEach([this, &StatBlock](const FStat &Stat) {
        if (Stat.BaseAttribute.IsValid() && Stat.Type != EPokemonStatType::Battle) {
            auto StatValue = StatBlock->GetStat(Stat.ID);
            BattlerAbilityComponent->SetNumericAttributeBase(Stat.BaseAttribute,
                                                             static_cast<float>(StatValue->GetStatValue()));
        }
    });

    BattlerAbilityComponent->SetNumericAttributeBase(UPokemonCoreAttributeSet::GetHPAttribute(),
                                                     static_cast<float>(WrappedPokemon->GetCurrentHP()));
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

bool ABattlerActor::CanGainExp() const {
    return OwningSide == OwningSide->GetOwningBattle()->GetPlayerSide();
}

float ABattlerActor::GetExpPercent() const {
    return WrappedPokemon->GetStatBlock()->GetExpPercent();
}

TArray<FExpGainInfo> ABattlerActor::GiveExpToParticipants() {
    TArray<FExpGainInfo> GainInfos;
    auto &Battle = OwningSide->GetOwningBattle();
    if (OwningSide != Battle->GetOpposingSide()) {
        return GainInfos;
    }

    auto &BattleSettings = *GetDefault<UPokemonBattleSettings>();
    auto &Species = WrappedPokemon->GetSpecies();

    float TrainerBoost =
        IBattle::Execute_IsTrainerBattle(Battle.GetObject()) ? BattleSettings.TrainerExpGainMultiplier : 1.f;
    int32 BaseExp = Species.BaseExp;
    int32 Level = GetPokemonLevel();

    auto &PlayerTrainer = UTrainerHelpers::GetPlayerCharacter(this);
    for (auto &PlayerParty = Battle->GetPlayerSide()->GetTrainerParty(PlayerTrainer); auto &Battler : PlayerParty) {
        int32 BattlerLevel = Battler->GetPokemonLevel();
        auto &ExpAttributes = *Battler->GetAbilityComponent()->GetExpAttributeSet();
        float SplitFactor = Participants.Contains(Battler->GetInternalId()) ? 1.f : 2.f;

        int32 ExpGain = FMath::FloorToInt32((BaseExp * Level / 5.f * TrainerBoost * (1.f / SplitFactor) *
                                                 FMath::Pow((2.f * Level + 10) / (Level + BattlerLevel + 10), 2.5f) +
                                             1) *
                                            ExpAttributes.GetExpGainRate());

        auto &GainInfo = GainInfos.Emplace_GetRef(Battler, ExpGain);
        GainInfo.StatChanges = Battler->GainExpAndEVs(GainInfo.Amount, Species.EVs);
    }

    return GainInfos;
}

FLevelUpStatChanges ABattlerActor::GainExpAndEVs(int32 Exp, const TMap<FName, uint8> &EVs) {
    // TODO: Actually gain EVs
    return WrappedPokemon->GetStatBlock()->GainExp(Exp, false);
}

TArray<FName> ABattlerActor::GetTypes() const {
    return WrappedPokemon->GetTypes();
}

UBattlerAbilityComponent *ABattlerActor::GetAbilityComponent() const {
    return BattlerAbilityComponent;
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

void ABattlerActor::RecordParticipation() {
    auto NewParticipants =
        OwningSide->GetOwningBattle()->GetSides() |
        ranges::views::filter([this](const TScriptInterface<IBattleSide> &Side) { return Side != OwningSide; }) |
        ranges::views::transform(
            [](const TScriptInterface<IBattleSide> &Side) { return RangeHelpers::CreateRange(Side->GetBattlers()); }) |
        ranges::views::join |
        ranges::views::filter([](const TScriptInterface<IBattler> &Battler) { return !Battler->IsFainted(); }) |
        ranges::views::filter([](const TScriptInterface<IBattler> &Battler) { return Battler->CanGainExp(); }) |
        ranges::views::transform([](const TScriptInterface<IBattler> &Battler) { return Battler->GetInternalId(); });
    ranges::for_each(NewParticipants, [this](const FGuid &ID) { Participants.Add(ID); });
}

const TOptional<FStatusEffectInfo> &ABattlerActor::GetStatusEffect() const {
    return StatusEffect;
}

void ABattlerActor::InflictStatusEffect(FName StatusEffectID, FActiveGameplayEffectHandle EffectHandle) {
    check(!StatusEffect.IsSet())
    StatusEffect.Emplace(StatusEffectID, EffectHandle);
}

void ABattlerActor::CureStatusEffect() {
    check(StatusEffect.IsSet())
    StatusEffect.Reset();
}

void ABattlerActor::UpdateHPValue(const FOnAttributeChangeData &Data) const {
    WrappedPokemon->SetCurrentHP(FMath::FloorToInt32(Data.NewValue));
}

void ABattlerActor::SpawnSpriteActor(bool ShouldShow) {
    Sprite = GetWorld()->SpawnActor<AActor>(BattlerSpriteClass.LoadSynchronous(), GetTransform());
    Sprite->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    IBattlerSprite::Execute_SetBattleSprite(
        Sprite, GraphicsSubsystem->GetPokemonBattleSprite(WrappedPokemon, this, OwningSide->ShowBackSprites()));
    Sprite->SetActorHiddenInGame(!ShouldShow);
}