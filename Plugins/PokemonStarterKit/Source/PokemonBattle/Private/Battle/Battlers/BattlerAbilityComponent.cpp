// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Attributes/TargetDamageStateAttributeSet.h"

void UBattlerAbilityComponent::BeginPlay() {
    Super::BeginPlay();

    CoreAttributes = NewObject<UPokemonCoreAttributeSet>(GetOwner());
    AddSpawnedAttribute(CoreAttributes);

    StatStagesAttributeSet = NewObject<UStatStagesAttributeSet>(GetOwner());
    AddSpawnedAttribute(StatStagesAttributeSet);

    TargetDamageStateAttributeSet = NewObject<UTargetDamageStateAttributeSet>(GetOwner());
    AddSpawnedAttribute(TargetDamageStateAttributeSet);
}

UPokemonCoreAttributeSet *UBattlerAbilityComponent::GetCoreAttributes() const {
    return CoreAttributes;
}

UStatStagesAttributeSet * UBattlerAbilityComponent::GetStatStages() const {
    return StatStagesAttributeSet;
}

UTargetDamageStateAttributeSet * UBattlerAbilityComponent::GetTargetDamageStateAttributeSet() const {
    return TargetDamageStateAttributeSet;
}

FGameplayEffectSpecHandle UBattlerAbilityComponent::MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                                                     float Level, FGameplayEffectContextHandle Context) const {
    if (Context.IsValid() == false) {
        Context = MakeEffectContext();
    }

    if (GameplayEffectClass) {
        auto GameplayEffect = CreateGameplayEffect(GameplayEffectClass, Level, Context);
        auto NewSpec = MakeUnique<FGameplayEffectSpec>(GameplayEffect, Context, Level);
        return FGameplayEffectSpecHandle(NewSpec.Release());
    }

    return FGameplayEffectSpecHandle(nullptr);
}

UGameplayEffect * UBattlerAbilityComponent::CreateGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                                                 float Level, const FGameplayEffectContextHandle& Context) const {
    auto GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
    if (GameplayEffect->DurationPolicy == EGameplayEffectDurationType::Instant) {
        return GameplayEffect;
    }

    if (GameplayEffect->StackingType != EGameplayEffectStackingType::None && ModifierInfo->Stackables.Contains(GameplayEffectClass)) {
        GameplayEffect = ModifierInfo->Stackables[GameplayEffectClass].Get();
        if (GameplayEffect != nullptr) {
            return GameplayEffect;
        }
    }
    
    GameplayEffect = NewObject<UGameplayEffect>(Context.GetInstigator(), GameplayEffectClass);
    if (GameplayEffect->StackingType != EGameplayEffectStackingType::None) {
        ModifierInfo->Stackables.Emplace(GameplayEffectClass, GameplayEffect);
    }

    for (auto &Modifier : GameplayEffect->Modifiers) {
        if (!Modifier.Attribute.IsValid()) {
            continue;
        }
            
        auto &[MultiplierCount, DivisorCount] = ModifierInfo->ModifierCount.FindOrAdd(Modifier.Attribute);
        if (Modifier.ModifierOp == EGameplayModOp::Multiplicitive) {
            check(MultiplierCount < TNumericLimits<uint8>::Max())
            Modifier.EvaluationChannelSettings.SetEvaluationChannel(static_cast<EGameplayModEvaluationChannel>(MultiplierCount));
            MultiplierCount++;
        } else if (Modifier.ModifierOp != EGameplayModOp::Division) {
            check(DivisorCount < TNumericLimits<uint8>::Max())
            Modifier.EvaluationChannelSettings.SetEvaluationChannel(static_cast<EGameplayModEvaluationChannel>(DivisorCount));
            DivisorCount++;
        }
    }

    return GameplayEffect;
}