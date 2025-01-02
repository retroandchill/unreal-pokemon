// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerActor.h"
#include "AbilitySystemBlueprintLibrary.h"
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
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Battle/Events/SwitchPokemonPayload.h"
#include "Battle/Items/ItemLookup.h"
#include "Battle/Moves/MoveLookup.h"
#include "Battle/Moves/PokemonBattleMove.h"
#include "Battle/Status.h"
#include "Battle/StatusEffects/StatusEffectLookup.h"
#include "Battle/StatusEffects/StatusEffectTags.h"
#include "Battle/Switching/SwitchActionBase.h"
#include "Battle/Tags.h"
#include "DataManager.h"
#include "Graphics/SpriteLoader.h"
#include "Pokemon/Abilities/AbilityBlock.h"
#include "Pokemon/Moves/Move.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "PokemonBattleSettings.h"






#include "Species/PokemonStatType.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"
#include "Utilities/TrainerHelpers.h"
#include <functional>

TScriptInterface<IBattleMove> CreateBattleMove(const TScriptInterface<IMove> &Move, ABattlerActor *Battler) {
    check(Battler != nullptr)
    check(Move != nullptr)
    TScriptInterface<IBattleMove> BattleMove = NewObject<UPokemonBattleMove>(Battler);
    BattleMove->Initialize(Battler, Move);
    return BattleMove;
}

ABattlerActor::ABattlerActor() {
    BattlerAbilityComponent = CreateDefaultSubobject<UBattlerAbilityComponent>("BattlerAbilityComponent");
    TurnBasedEffectComponent = CreateDefaultSubobject<UTurnBasedEffectComponent>(FName("TurnBasedEffectsComponent"));
    InnateAbilities.Add(UInnate_CriticalHitDamage::StaticClass());
    InnateAbilities.Add(UInnate_MultiTargetDamageSplit::StaticClass());
    InnateAbilities.Add(UInnate_DamageSwing::StaticClass());
}

TScriptInterface<IBattler> ABattlerActor::Initialize(const TScriptInterface<IBattleSide> &Side,
                                                     const TScriptInterface<IPokemon> &Pokemon, bool ShowImmediately) {
    OwningSide = Side;
    WrappedPokemon = Pokemon;
    InternalId = FGuid::NewGuid();
    OwningTrainer = WrappedPokemon->GetCurrentHandler();

    auto &DataSubsystem = FDataManager::GetInstance();
    auto &StatTable = DataSubsystem.GetDataTable<FStat>();

    TMap<UClass *, UAttributeSet *> Attributes;
    for (auto AttributeSets = BattlerAbilityComponent->GetSpawnedAttributes(); auto Attribute : AttributeSets) {
        Attributes.Add(Attribute->GetClass(), Attribute);
    }

    auto StatBlock = WrappedPokemon->GetStatBlock();
    StatTable.GetAllRows() | Retro::Ranges::ForEach([this, &Attributes, &StatBlock](const FStat &Stat) {
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
    // clang-format off
    Moves = MoveBlock->GetMoves() |
            Retro::Ranges::Views::Transform<&CreateBattleMove>(this) |
            Retro::Ranges::To<TArray>();
    // clang-format on
    SpawnSpriteActor(ShowImmediately);

    auto &Battle = OwningSide->GetOwningBattle();
    if (OwningSide->ShowBackSprites()) {
        Controller = NewObject<UPlayerBattlerController>(this)->SetBattle(Battle);
    } else {
        Controller = NewObject<UAIBattlerController>(this);
    }
    Controller->BindOnActionReady(
        FActionReady::CreateLambda(std::bind_front(&IBattle::QueueAction, Battle.GetInterface())));

    if (auto AbilityClass = Pokemon::Battle::Abilities::CreateAbilityEffect(Pokemon->GetAbility()->GetAbilityID());
        AbilityClass.IsSet()) {
        Ability =
            BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass.GetValue(), 1, INDEX_NONE, this));
    } else {
        Ability = FGameplayAbilitySpecHandle();
    }

    if (auto HoldItemClass = Pokemon::Battle::Items::FindHoldItemEffect(Pokemon->GetHoldItem().GetPtrOrNull());
        HoldItemClass.IsSet()) {
        HoldItem =
            BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(HoldItemClass.GetValue(), 1, INDEX_NONE, this));
    } else {
        HoldItem = FGameplayAbilitySpecHandle();
    }

    if (auto StatusEffectClass = Pokemon::Battle::StatusEffects::FindStatusEffect(Pokemon->GetStatusEffect());
        StatusEffectClass.IsSet()) {
        auto Spec = BattlerAbilityComponent->MakeOutgoingSpec(StatusEffectClass.GetValue(), 0,
                                                              BattlerAbilityComponent->MakeEffectContext());
        StatusEffect.Emplace(Pokemon->GetStatusEffect()->ID,
                             BattlerAbilityComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data));
    } else {
        StatusEffect.Reset();
    }

    auto SwitchActionAbility =
        GetDefault<UPokemonBattleSettings>()->SwitchAbilityClass.TryLoadClass<USwitchActionBase>();
    SwitchActionHandle =
        BattlerAbilityComponent->GiveAbility(FGameplayAbilitySpec(SwitchActionAbility, 1, INDEX_NONE, this));

    return this;
}

void ABattlerActor::BeginPlay() {
    Super::BeginPlay();
    BattlerAbilityComponent->InitAbilityActorInfo(this, this);
    // clang-format off
    InnateAbilityHandles = InnateAbilities |
                           Retro::Ranges::Views::Transform(Retro::Construct<FGameplayAbilitySpec>, 1, INDEX_NONE, this) |
                           Retro::Ranges::Views::Transform(BattlerAbilityComponent, &UAbilitySystemComponent::GiveAbility) |
                           Retro::Ranges::To<TArray>();
    InnateEffectHandles = InnateEffects |
                          Retro::Ranges::Views::Transform([this](const TSubclassOf<UGameplayEffect> &Effect) {
                              auto Context = BattlerAbilityComponent->MakeEffectContext();
                              auto SpecHandle = BattlerAbilityComponent->MakeOutgoingSpec(Effect, 1, Context);
                              return BattlerAbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
                          }) |
                          Retro::Ranges::To<TArray>();
    // clang-format on
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

bool ABattlerActor::IsActive() const {
    return OwningSide->GetBattlers().Contains(this);
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
    StatTable.GetAllRows() | Retro::Ranges::ForEach([this, &StatBlock](const FStat &Stat) {
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

bool ABattlerActor::IsNotFainted() const {
    return !WrappedPokemon->IsFainted();
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
        if (Battler->IsFainted()) {
            auto &GainInfo = GainInfos.Emplace_GetRef(Battler, 0);
            GainInfo.StatChanges = Battler->GainExpAndEVs(GainInfo.Amount, {});
            continue;
        }

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
    auto StatBlock = WrappedPokemon->GetStatBlock();
    for (auto &[ID, Amount] : EVs) {
        auto Stat = StatBlock->GetStat(ID);
        Stat->SetEV(Stat->GetEV() + Amount);
    }
    return StatBlock->GainExp(Exp, false);
}

TArray<FName> ABattlerActor::GetTypes() const {
    return WrappedPokemon->GetTypes();
}

UBattlerAbilityComponent *ABattlerActor::GetAbilityComponent() const {
    return BattlerAbilityComponent;
}

UTurnBasedEffectComponent *ABattlerActor::GetTurnBasedEffectComponent() const {
    return TurnBasedEffectComponent;
}

const TArray<TScriptInterface<IBattleMove>> &ABattlerActor::GetMoves() const {
    return Moves;
}

FText ABattlerActor::GetRecallMessage() const {
    return GetMessageOnRecall();
}

FGameplayAbilitySpecHandle ABattlerActor::PerformSwitch(const TScriptInterface<IBattler> &SwitchTarget) {
    FGameplayEventData EventData;
    EventData.Instigator = this;

    check(OwningTrainer != nullptr)
    auto &TrainerParty = OwningSide->GetTrainerParty(OwningTrainer);
    auto Payload = NewObject<USwitchPokemonPayload>();
    Payload->OwningTrainer = OwningTrainer;
    Payload->UserIndex = TrainerParty.Find(this);
    Payload->SwapIndex = TrainerParty.Find(SwitchTarget);
    check(Payload->UserIndex != INDEX_NONE && Payload->SwapIndex != INDEX_NONE)

    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->SetActors({CastChecked<AActor>(SwitchTarget.GetObject())});
    EventData.TargetData.Data.Emplace(TargetData);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Pokemon::Battle::SwitchOut, EventData);
    return SwitchActionHandle;
}

bool ABattlerActor::IsOwnedByPlayer() const {
    return WrappedPokemon->GetCurrentHandler() == UTrainerHelpers::GetPlayerCharacter(this);
}

void ABattlerActor::SelectActions() {
    Controller->InitiateActionSelection(this);
}

void ABattlerActor::RequireSwitch() {
    Controller->InitiateForcedSwitch(this);
}

uint8 ABattlerActor::GetActionCount() const {
    return 1;
}

Retro::Ranges::TAnyView<TScriptInterface<IBattler>> ABattlerActor::GetAllies() const {
    // clang-format off
    return OwningSide->GetBattlers() |
           Retro::Ranges::Views::Filter([this](const TScriptInterface<IBattler> &Battler) {
               return Battler->GetInternalId() == InternalId;
           });
    // clang-format on
}

void ABattlerActor::ShowSprite(const FVector &Offset) const {
    check(Sprite != nullptr)
    Sprite->SetActorLocation(Sprite->GetActorLocation() + Offset);
    Sprite->SetActorHiddenInGame(false);
}

void ABattlerActor::HideSprite() const {
    check(Sprite != nullptr)
    Sprite->SetActorHiddenInGame(true);
}

void ABattlerActor::RecordParticipation() {
    if (!IsOwnedByPlayer()) {
        return;
    }

    OwningSide->GetOwningBattle()->GetOpposingSide()->GetBattlers() | Retro::Ranges::Views::Filter<&IBattler::IsNotFainted>() |
        Retro::Ranges::ForEach(&IBattler::AddParticipant, this);
}

void ABattlerActor::AddParticipant(const TScriptInterface<IBattler> &Participant) {
    Participants.Add(Participant->GetInternalId());
}

int32 ABattlerActor::GetTurnCount() const {
    return TurnCount;
}

const TOptional<FStatusEffectInfo> &ABattlerActor::GetStatusEffect() const {
    return StatusEffect;
}

void ABattlerActor::InflictStatusEffect(FStatusHandle StatusEffectID, FActiveGameplayEffectHandle EffectHandle) {
    check(!StatusEffect.IsSet())
    StatusEffect.Emplace(StatusEffectID, EffectHandle);
    WrappedPokemon->SetStatusEffect(StatusEffectID);
}

void ABattlerActor::CureStatusEffect() {
    check(StatusEffect.IsSet())
    StatusEffect.Reset();
    WrappedPokemon->RemoveStatusEffect();
}

FTransform ABattlerActor::GetSpriteTransform_Implementation() const {
    return GetTransform();
}

void ABattlerActor::UpdateHPValue(const FOnAttributeChangeData &Data) const {
    WrappedPokemon->SetCurrentHP(FMath::FloorToInt32(Data.NewValue));
}

void ABattlerActor::SpawnSpriteActor(bool ShouldShow) {
    Sprite = GetWorld()->SpawnActor<AActor>(BattlerSpriteClass.LoadSynchronous(), GetSpriteTransform());
    Sprite->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

    IBattlerSprite::Execute_SetBattleSprite(
        Sprite, USpriteLoader::GetPokemonBattleSprite(WrappedPokemon, OwningSide->ShowBackSprites()));
    Sprite->SetActorHiddenInGame(!ShouldShow);
}