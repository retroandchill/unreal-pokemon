// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleDamage.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoveEvaluationHelpers.generated.h"

struct FAdditionalMoveEffects;

/**
 * Struct that wraps around a battle stat selection
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleStat {
    GENERATED_BODY()

    /**
     * The actual value of the stat change in the editor
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats",
        meta = (GetOptions = "PokemonData.StatHelper.GetBattleStatNames"))
    FName Stat;
};

/**
 * Function used to get the type hash of the battle stat, making it identical to the wrapped property.
 * @param Key The key structure
 * @return The return type in question
 */
inline uint32 GetTypeHash(const FBattleStat &Key) {
    return GetTypeHash(Key.Stat);
}

/**
 * Helper library for move related evaluation.
 */
UCLASS()
class POKEMONBATTLE_API UMoveEvaluationHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Take an existing critical override value and change it based on the new value and the existing rules for precedence.
     * @param Old The old value
     * @param New The new value to attempt to assign to
     * @return The result after evaluation
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Critical Hits")
    static ECriticalOverride ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New);

    /**
     * Alter the stat stages of the user.
     * @param Battler The battler the effect is being applied to
     * @param Effects The result to write the change to.
     * @param Stat The stat that is being altered
     * @param Stages The change to the stat value
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Stats")
    static void AlterStatStages(const TScriptInterface<IBattler>& Battler, UPARAM(Ref) FAdditionalMoveEffects& Effects, const FBattleStat& Stat, int32
                                Stages);

};