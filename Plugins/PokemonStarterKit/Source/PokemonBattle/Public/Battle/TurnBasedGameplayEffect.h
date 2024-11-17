// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "UObject/Object.h"

#include "TurnBasedGameplayEffect.generated.h"

class UTurnBasedEffectComponent;

/**
 * @struct FTurnBasedGameplayEffect
 * @brief A structure that represents a turn-based gameplay effect.
 *
 * This struct handles a gameplay effect that has a lifespan defined in turns. It keeps track of the number
 * of turns for which the effect has been active and manages its duration.
 */
USTRUCT()
struct POKEMONBATTLE_API FTurnBasedGameplayEffect {
    GENERATED_BODY()

    /**
     * @brief Default constructor for FTurnBasedGameplayEffect.
     *
     * This constructor creates a new instance of FTurnBasedGameplayEffect with default
     * initialization of its member variables.
     *
     * @return A default-constructed instance of FTurnBasedGameplayEffect.
     */
    FTurnBasedGameplayEffect() = default;

    /**
     * @brief Constructs a FTurnBasedGameplayEffect with the specified parameters.
     *
     * Initializes a new turn-based gameplay effect with the given owning component, effect handle,
     * and turn duration. The effect tracks the number of turns remaining for its lifespan.
     *
     * @param OwningComponent Pointer to the owning UTurnBasedEffectComponent.
     * @param EffectHandle Handle to the active gameplay effect.
     * @param TurnDuration Number of turns the effect should last. Defaults to INDEX_NONE.
     * @return A new instance of FTurnBasedGameplayEffect.
     */
    FTurnBasedGameplayEffect(UTurnBasedEffectComponent *OwningComponent, FActiveGameplayEffectHandle EffectHandle, int32 TurnDuration = INDEX_NONE);

    /**
     * @brief Increments the turn count for the gameplay effect.
     *
     * This method increments the counter for the number of turns the gameplay effect has been active.
     * It then checks if the effect's remaining turns are less than or equal to the current active turns
     * and removes the effect if necessary.
     *
     * @return True if the effect is removed after the increment; otherwise, false.
     */
    bool IncrementTurnCount();
    

private:
    bool RemoveEffect(int32 StacksToRemove = -1);
    
    UPROPERTY()
    TWeakObjectPtr<UTurnBasedEffectComponent> OwningComponent;
    
    UPROPERTY()
    FActiveGameplayEffectHandle EffectHandle;
    
    UPROPERTY()
    int32 TurnsActive = 0;

    UPROPERTY()
    TOptional<int32> TurnsRemaining;

};
