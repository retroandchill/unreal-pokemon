// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "PaperSprite.h"

#include "Type.generated.h"

/**
 * The various damage effectiveness that there are. Used to determine what message to show to the player.
 */
UENUM(BlueprintType)
enum class EDamageEffectiveness : uint8 {
    /**
     * Move deals no damage, but was also non-damaging to begin with
     * TODO: Remove this once the ties to this enum in the old battle code have been severed
     */
    NonDamaging = 0x0 UMETA(Hidden),

    /**
     * The move deals no damage
     */
    NoEffect = 0x1 << 0,
    
    /**
     * The move deals less damage
     */
    NotVeryEffective = 0x1 << 1,
    
    /**
     * The move deals normal damage
     */
    Normal = 0x1 << 2,

    /**
     * The move deals more damage.
     */
    SuperEffective = 0x1 << 3
};

namespace Pokemon::TypeEffectiveness {

/**
 * The amount to multiply a move's damage by for an immunity
 */
constexpr float IneffectiveMultiplier = 0.0f;

/**
 * The amount to multiply a move's damage by for a resistance
 */
constexpr float NotVeryEffectiveMultiplier = 0.5f;

/**
 * The amount of multiply a move's damage by when it does normal damage.
 */
constexpr float NormalEffectiveMultiplier = 1.0f;

/**
 * The amount to multiply a move's damage by for a weakness
 */
constexpr float SuperEffectiveMultiplier = 2.0f;

/**
 * Is the match-up against the move ineffective?
 * @param Multiplier The multiplier for the determined match-up
 * @return Is the match-up against the move ineffective?
 */
constexpr bool IsIneffective(float Multiplier) {
    return Multiplier <= IneffectiveMultiplier;
}

/**
 * Is the match-up against the move not-very-effective?
 * @param Multiplier The multiplier for the determined match-up
 * @return Is the match-up against the move not-very-effective?
 */
constexpr bool IsNotVeryEffective(float Multiplier) {
    return Multiplier > IneffectiveMultiplier && Multiplier < NormalEffectiveMultiplier;
}

/**
 * Is the match-up against the move not-very-effective or ineffective?
 * @param Multiplier The multiplier for the determined match-up
 * @return Is the match-up against the move not-very-effective or ineffective?
 */
constexpr bool IsResistant(float Multiplier) {
    return Multiplier < NormalEffectiveMultiplier;
}

/**
 * Is the match-up against the move normally effectiveness?
 * @param Multiplier The multiplier for the determined match-up
 * @return Is the match-up against the move normally effectiveness?
 */
constexpr bool IsNormal(float Multiplier) {
    return Multiplier == NormalEffectiveMultiplier;
}

/**
 * Is the match-up against the move super effective?
 * @param Multiplier The multiplier for the determined match-up
 * @return Is the match-up against the move super effective?
 */
constexpr bool IsSuperEffective(float Multiplier) {
    return Multiplier > NormalEffectiveMultiplier;
}

/**
 * Get the effectiveness multiplier for the given effectiveness value
 * @param Effectiveness The effectiveness to evaluate
 * @return The multiplier to apply
 */
constexpr float GetMultiplierForEffectiveness(EDamageEffectiveness Effectiveness) {
    using enum EDamageEffectiveness;
    switch (Effectiveness) {
    case NoEffect:
        return IneffectiveMultiplier;
    case NotVeryEffective:
        return NotVeryEffectiveMultiplier;
    case SuperEffective:
        return SuperEffectiveMultiplier;
    default:
        return NormalEffectiveMultiplier;
    }
}

/**
 * Get the effectiveness amount from the provided multiplier
 * @param Multiplier The multiplier for the determined match-up
 * @return The state of effectiveness
 */
constexpr EDamageEffectiveness GetEffectivenessFromMultiplier(float Multiplier) {
    using enum EDamageEffectiveness;

    if (IsIneffective(Multiplier)) {
        return NoEffect;
    }

    if (IsNotVeryEffective(Multiplier)) {
        return NotVeryEffective;
    }

    if (IsSuperEffective(Multiplier)) {
        return SuperEffective;
    }

    return Normal;
}

}

/**
 * Represents the types that belong to both individual species as well as the moves used by them
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
struct POKEMONDATA_API FType : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * Graphic that represents a sprite in the menu
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals")
    UPaperSprite *Icon;

    /**
     * Is this type special? (Only means anything when the Physical/Special-split is disabled)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Type Info")
    bool IsSpecialType = false;

    /**
     * Is this type only a type used by moves amd not one that a Pokémon can be.
     *
     * Examples of this would include the "???" Type used by Curse in generation II-IV and Shadow Moves.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Type Info")
    bool IsPseudoType = false;

    /**
     * Move types that deal super-effective damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Weaknesses;

    /**
     * Move types that deal not very effective damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Resistances;

    /**
     * Move types that deal no damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Immunities;

    /**
     * Words/phrases that can be used to group certain kinds of types"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    TArray<FName> Tags;

    /**
     * Get this type's effectiveness against the opposing type
     * @param OtherType The other type to evaluate against
     * @return The effectiveness against the other type
     */
    float Effectiveness(FName OtherType) const;
    
};

/**
 * Blueprint function library for getting type data out.
 */
UCLASS()
class POKEMONDATA_API UTypeHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible type names.
     * @return The list of all possible type names.
     */
    UFUNCTION()
    static TArray<FName> GetTypeNames();

    /**
     * Get the effectiveness of the attacking type against the defending type
     * @param AttackType The attacking type
     * @param DefendType The defending type
     * @return The amount to multiply the damage by
     */
    UFUNCTION(BlueprintPure, Category = "Types|Effectiveness")
    static float GetTypeEffectiveness(FName AttackType, FName DefendType);
};
