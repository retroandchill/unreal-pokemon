// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/StatStagesAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/TargetDamageStateAttributeSet.h"

void UBattlerAbilityComponent::BeginPlay() {
    Super::BeginPlay();

    CoreAttributes = NewObject<UPokemonCoreAttributeSet>(this);
    AddSpawnedAttribute(CoreAttributes);

    StatStagesAttributeSet = NewObject<UStatStagesAttributeSet>(this);
    AddSpawnedAttribute(StatStagesAttributeSet);

    TargetDamageStateAttributeSet = NewObject<UTargetDamageStateAttributeSet>(this);
    AddSpawnedAttribute(TargetDamageStateAttributeSet);
}

UPokemonCoreAttributeSet *UBattlerAbilityComponent::GetCoreAttributes() const {
    return CoreAttributes;
}

UStatStagesAttributeSet * UBattlerAbilityComponent::GetStatStages() const {
    return StatStagesAttributeSet;
}

UMoveUsageAttributeSet * UBattlerAbilityComponent::GetMoveUsageAttributeSet() const {
    return MoveUsageAttributeSet;
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
        auto NewSpec = new FGameplayEffectSpec(GameplayEffect, Context, Level);
        return FGameplayEffectSpecHandle(NewSpec);
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