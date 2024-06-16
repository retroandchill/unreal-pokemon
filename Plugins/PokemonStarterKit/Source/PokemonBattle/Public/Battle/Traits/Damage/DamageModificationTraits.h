// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
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
};