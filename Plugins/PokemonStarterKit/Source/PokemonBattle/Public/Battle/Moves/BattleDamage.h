// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "BattleDamage.generated.h"

/**
 * The various damage effectivenesses that there are. Used to determine what message to show to the player.
 */
UENUM(BlueprintType)
enum class EDamageEffectiveness : uint8 {
    /**
     * Move deals no damage, but was also non-damaging to begin with
     */
    NonDamaging,

    /**
     * The move deals normal damage
     */
    Normal,

    /**
     * The move deals more damage.
     */
    SuperEffective,

    /**
     * The move deals less damage
     */
    NotVeryEffective,

    /**
     * The move deals no damage
     */
    NoEffect
};

/**
 * Struct that contains the results of the damage calculation
 */
USTRUCT(BlueprintType)
struct FBattleDamage {
    GENERATED_BODY()

    /**
     * The damage dealt by the move.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    int32 Damage = 0;

    /**
     * The effectiveness of the damage.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    EDamageEffectiveness Effectiveness = EDamageEffectiveness::NonDamaging;

    /**
     * Did this move score a critical hit
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    bool bCriticalHit;
};

/**
 * Represents any calculated move effect modifiers
 */
USTRUCT(BlueprintType)
struct FDamageEffects {
    GENERATED_BODY()

    /**
     * The actual type matchup value
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float TypeMatchUp = 1.0f;

    /**
     * The effectiveness of the damage.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    EDamageEffectiveness Effectiveness = EDamageEffectiveness::Normal;

    /**
     * Did this move score a critical hit
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    bool bCriticalHit = false;
};

/**
 * Represents the attack and defense to use for a damage calculation.
 */
USTRUCT(BlueprintType)
struct FAttackAndDefense {
    GENERATED_BODY()

    /**
     * The attack stat to use
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    int32 Attack;

    /**
     * The defense stat to use
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    int32 Defense;
};

/**
 * Represents the damage multipliers for moves
 */
USTRUCT(BlueprintType)
struct FDamageMultipliers {
    GENERATED_BODY()

    /**
     * The factor to multiply the move's power by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float PowerMultiplier = 1.0f;

    /**
     * The factor to multiply the user's attack by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float AttackMultiplier = 1.0f;

    /**
     * The factor to multiply the user's defense by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float DefenseMultiplier = 1.0f;

    /**
     * The factor to multiply the final damage output by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float FinalDamageMultiplier = 1.0f;
};