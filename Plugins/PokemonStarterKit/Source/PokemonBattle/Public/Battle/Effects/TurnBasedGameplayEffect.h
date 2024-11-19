// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "UObject/Object.h"
#include "Ranges/Optional/OptionalRef.h"

#include "TurnBasedGameplayEffect.generated.h"

class UGameplayEffect;
class UTurnBasedEffectComponent;

/**
 * @enum ETurnDurationTrigger
 * @brief Enum representing different trigger points in a turn-based game.
 *
 * This enumeration defines specific points within a turn or an action that can be used to
 * trigger gameplay events or effects, such as the start or end of a turn or action.
 */
UENUM(BlueprintType)
enum class ETurnDurationTrigger : uint8 {
    /**
     * Triggered at the end of the turn, will remove the effect before any turn-end abilities are triggered.
     */
    TurnEnd,

    /**
     * Triggered before the battler's action.
     */
    ActionStart,

    /**
     * Triggered after the battler's action.
     */
    ActionEnd
};

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
     * @brief Retrieves the owning component of the turn-based effect.
     *
     * This method returns an optional reference to the UTurnBasedEffectComponent that owns this
     * gameplay effect. If the owning component is valid, it will be returned; otherwise, the optional
     * will be empty.
     *
     * @return An optional reference to the owning UTurnBasedEffectComponent.
     */
    TOptional<UTurnBasedEffectComponent&> GetOwningComponent() const {
        return OwningComponent.Get();
    }

    /**
     * @brief Retrieves the handle of the active gameplay effect.
     *
     * This method returns a constant reference to the FActiveGameplayEffectHandle
     * associated with the turn-based gameplay effect.
     *
     * @return A constant reference to the FActiveGameplayEffectHandle.
     */
    const FActiveGameplayEffectHandle & GetEffectHandle() const {
        return EffectHandle;
    }

    /**
     * @brief Retrieves the gameplay effect associated with the turn-based gameplay component.
     *
     * This method returns a constant pointer to the UGameplayEffect associated with the
     * turn-based gameplay component. The gameplay effect defines various properties and behaviors
     * of the gameplay effect that this component is handling.
     *
     * @return A constant pointer to the UGameplayEffect.
     */
    const UGameplayEffect* GetGameplayEffect() const {
        return GameplayEffect;
    }

    /**
     * @brief Retrieves the number of turns the gameplay effect has been active.
     *
     * This method returns the count of how many turns the gameplay effect has been active.
     *
     * @return The number of turns the gameplay effect has been active.
     */
    int32 GetTurnsActive() const {
        return TurnsActive;
    }

    /**
     * @brief Retrieves the number of turns remaining for the gameplay effect.
     *
     * This method returns a constant reference to an optional integer representing the number of turns
     * remaining for this gameplay effect. If the effect has no remaining turns, the optional will be empty.
     *
     * @return A constant reference to an optional integer containing the number of turns remaining.
     */
    const TOptional<int32> & GetTurnsRemaining() const {
        return TurnsRemaining;
    }

    /**
     * @brief Checks if the turn-based gameplay effect has a specified trigger.
     *
     * This method determines if the gameplay effect contains a specific turn duration trigger,
     * such as triggers that may occur at the start or end of a turn or action.
     *
     * @param Trigger The turn duration trigger to check for.
     * @return True if the trigger exists within the gameplay effect; otherwise, false.
     */
    bool HasTrigger(ETurnDurationTrigger Trigger) const;

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
    TObjectPtr<const UGameplayEffect> GameplayEffect;
    
    UPROPERTY()
    int32 TurnsActive = 0;

    UPROPERTY()
    TOptional<int32> TurnsRemaining;
};
