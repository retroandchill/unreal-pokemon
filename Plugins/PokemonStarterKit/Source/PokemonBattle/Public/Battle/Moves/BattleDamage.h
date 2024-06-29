// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Type.h"
#include "Battle/Battlers/BattlerStats.h"
#include "UObject/Interface.h"

#include "BattleDamage.generated.h"

class UDamageModificationAttributeSet;
class IBattler;

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

USTRUCT()
struct POKEMONBATTLE_API FDamageResult {
    GENERATED_BODY()

    TSharedPtr<int32> Damage = MakeShared<int32>(0);
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
    FMainBattleStat Attack;

    /**
     * The defense stat to use
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    FMainBattleStat Defense;
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

    /**
     * Multiplies each individual multiplier by each other
     * @param Other The other multiplier to apply this towards
     * @return
     */
    FORCEINLINE FDamageMultipliers &operator*=(const FDamageMultipliers &Other) {
        PowerMultiplier *= Other.PowerMultiplier;
        AttackMultiplier *= Other.AttackMultiplier;
        DefenseMultiplier *= Other.DefenseMultiplier;
        FinalDamageMultiplier *= Other.FinalDamageMultiplier;
        return *this;
    }
};

USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FDamageMultiplierHandler {
    GENERATED_BODY()

    TSharedRef<FDamageMultipliers> Multipliers = MakeShared<FDamageMultipliers>();

    FDamageMultiplierHandler();

    explicit FDamageMultiplierHandler(const UDamageModificationAttributeSet& Attributes);
};

/**
 * Information about a move during damage modification
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FMoveDamageInfo {
    GENERATED_BODY()

    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    TScriptInterface<IBattler> User;

    /**
     * The target of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    TScriptInterface<IBattler> Target;

    /**
     * The total number of targets being hit
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    int32 TargetCount = 1;

    /**
     * The calculated type of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    FName Type;

    /**
     * The calculated base damage value of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    int32 BaseDamage = 0;

    /**
     * Additional information about the damage that has been accounted for
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    FDamageEffects Effects;

    /**
     * Default constructor for the struct
     */
    FMoveDamageInfo();

    /**
     * Create a basic structure with the specified values
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The total number of available targets
     */
    FMoveDamageInfo(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount = 1);
};

UENUM()
enum class ECriticalOverride : uint8 {
    /**
     * Roll a critical hit normally.
     */
    Normal,

    /**
     * The move always rolls a critical hit
     */
    Always,

    /**
     * The move can never land a crit.
*/
    Never
    
};