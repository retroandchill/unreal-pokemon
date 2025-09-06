// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Battle/AsyncAbilityComponent.h"
#include <memory>

#include "BattlerAbilityComponent.generated.h"

class UMoveUsageAttributeSet;
class UTargetDamageStateAttributeSet;
class UStatStagesAttributeSet;
class UPokemonCoreAttributeSet;
class UExpAttributeSet;

struct FModifierCount
{
    uint8 MultiplierCount = 0;
    uint8 DivisorCount = 0;
};

struct FModifierInformation
{
    TMap<FGameplayAttribute, FModifierCount> ModifierCount;
    TMap<TSubclassOf<UGameplayEffect>, TWeakObjectPtr<UGameplayEffect>> Stackables;
};

UCLASS(ClassGroup = ("Gameplay Abilities"), meta = (BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattlerAbilityComponent : public UAsyncAbilityComponent
{
    GENERATED_BODY()

  public:
    void BeginPlay() override;

    UPokemonCoreAttributeSet *GetCoreAttributes() const;
    UStatStagesAttributeSet *GetStatStages() const;
    UTargetDamageStateAttributeSet *GetTargetDamageStateAttributeSet() const;
    UExpAttributeSet *GetExpAttributeSet() const;

    /**
     * Find the gameplay ability with the specified class.
     * @param AbilityClass The class of ability to look for.
     * @return The handle for said ability (if found)
     */
    TOptional<FGameplayAbilitySpecHandle> FindAbilityOfClass(TSubclassOf<UGameplayAbility> AbilityClass) const;

  private:
    UPROPERTY()
    TObjectPtr<UPokemonCoreAttributeSet> CoreAttributes;

    UPROPERTY()
    TObjectPtr<UStatStagesAttributeSet> StatStagesAttributeSet;

    UPROPERTY()
    TObjectPtr<UTargetDamageStateAttributeSet> TargetDamageStateAttributeSet;

    UPROPERTY()
    TObjectPtr<UExpAttributeSet> ExpAttributeSet;

    TUniquePtr<FModifierInformation> ModifierInfo = MakeUnique<FModifierInformation>();
};