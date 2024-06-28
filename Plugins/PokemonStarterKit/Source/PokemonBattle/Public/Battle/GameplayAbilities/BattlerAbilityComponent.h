// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include <memory>

#include "BattlerAbilityComponent.generated.h"


class UStatStagesAttributeSet;
class UPokemonCoreAttributeSet;

struct FModifierCount {
    uint8 MultiplierCount = 0;
    uint8 DivisorCount = 0;
};

struct FModifierInformation {
    TMap<FGameplayAttribute, FModifierCount> ModifierCount;
    TMap<TSubclassOf<UGameplayEffect>, TWeakObjectPtr<UGameplayEffect>> Stackables;
};


UCLASS(ClassGroup=("Gameplay Abilities"), meta=(BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattlerAbilityComponent : public UAbilitySystemComponent {
    GENERATED_BODY()
    
protected:
    void BeginPlay() override;

public:
    UPokemonCoreAttributeSet *GetCoreAttributes() const;
    UStatStagesAttributeSet *GetStatStages() const;

    FGameplayEffectSpecHandle MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const override;

private:
    UGameplayEffect* CreateGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, const FGameplayEffectContextHandle& Context) const;
    
    UPROPERTY()
    TObjectPtr<UPokemonCoreAttributeSet> CoreAttributes;

    UPROPERTY()
    TObjectPtr<UStatStagesAttributeSet> StatStagesAttributeSet;

    TUniquePtr<FModifierInformation> ModifierInfo = MakeUnique<FModifierInformation>();
    
};