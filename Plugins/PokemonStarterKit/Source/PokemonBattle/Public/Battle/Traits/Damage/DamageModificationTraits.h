// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/TraitScopes.h"
#include "UObject/Interface.h"

#include "DamageModificationTraits.generated.h"

class UDamageModificationTrait;

/**
 * Damage modification traits that apply to any trait holder that applies to a single entity
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FIndividualDamageModifierTraits {
    GENERATED_BODY()

    /**
     * Damage modification traits that apply on moves used by anyone in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> Global;

    /**
     * Damage modification traits that apply on moves used by the owner of the ability
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> User;

    /**
     * Damage modification traits that apply on moves used by any ally of the ability owner
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> UserAllies;

    /**
     * Damage modification traits that apply on moves that target the owner of the ability
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> Target;

    /**
     * Damage modification traits that apply on moves that target an ally of the ability owner
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> TargetAllies;

    template <EIndividualTraitScope Scope>
    FORCEINLINE const TArray<TObjectPtr<UDamageModificationTrait>>& GetTraitsForScope() const {
        if constexpr (Scope == EIndividualTraitScope::User) {
            return User;
        } else if constexpr (Scope == EIndividualTraitScope::UserAlly) {
            return UserAllies;
        } else if constexpr (Scope == EIndividualTraitScope::Target) {
            return Target;
        } else if constexpr (Scope == EIndividualTraitScope::TargetAlly) {
            return TargetAllies;
        } else { // Assume Global for invalid enum literals
            return Global;
        }
    }
};