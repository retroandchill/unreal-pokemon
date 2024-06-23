// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BattlerAbilityComponent.generated.h"


class UDamageModificationAttributeSet;

UCLASS(ClassGroup=("Gameplay Abilities"), meta=(BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattlerAbilityComponent : public UAbilitySystemComponent {
    GENERATED_BODY()

protected:
    /**
     * @copydoc UActorComponent::BeginPlay
     */
    void BeginPlay() override;

public:
    /**
     * Get the attributes held by the Pokémon for move damage modification
     * @return The attributes held by the Pokémon for move damage modification
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Attributes")
    UDamageModificationAttributeSet* GetDamageModificationAttributes() const;
    

private:
    /**
     * The attributes held by the Pokémon for move damage modification
     */
    UPROPERTY()
    TObjectPtr<UDamageModificationAttributeSet> DamageModificationAttributes;

};