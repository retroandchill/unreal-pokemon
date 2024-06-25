﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "Settings/BaseSettings.h"

#include "DamageModificationAttributeSet.generated.h"

struct FMoveDamageInfo;

/**
 * The set of attributes that a Pokémon has in battle.
 */
UCLASS()
class POKEMONBATTLE_API UDamageModificationAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UDamageModificationAttributeSet, PowerMultiplier)
    ATTRIBUTE_ACCESSORS(UDamageModificationAttributeSet, AttackMultiplier)
    ATTRIBUTE_ACCESSORS(UDamageModificationAttributeSet, DefenseMultiplier)
    ATTRIBUTE_ACCESSORS(UDamageModificationAttributeSet, FinalDamageMultiplier)
    ATTRIBUTE_ACCESSORS(UDamageModificationAttributeSet, CriticalHitMultiplier)

    /**
     * Resets all values on this attribute set to their original values prior to damage calculation
     */
    void Reset();
    
private:
    /**
     * The factor to multiply a move's power by
     */
    UPROPERTY()
    FGameplayAttributeData PowerMultiplier = 1.f;

    /**
     * The factor to multiply the user's attack by
     */
    UPROPERTY()
    FGameplayAttributeData AttackMultiplier = 1.f;

    /**
     * The factor to multiply the target's defense by
     */
    UPROPERTY()
    FGameplayAttributeData DefenseMultiplier = 1.f;

    /**
     * The factor to multiply the final damage output by
     */
    UPROPERTY()
    FGameplayAttributeData FinalDamageMultiplier = 1.f;

    /**
     * The factor to multiply the damage by in the event of a critical hit
     */
    UPROPERTY()
    FGameplayAttributeData CriticalHitMultiplier = Pokemon::FBaseSettings::Get().GetCriticalHitDamageMod();

};
