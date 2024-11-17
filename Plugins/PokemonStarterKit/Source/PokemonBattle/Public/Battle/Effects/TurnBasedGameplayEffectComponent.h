// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "TurnBasedGameplayEffectComponent.generated.h"

class UTurnBasedEffectComponent;
struct FGameplayEffectRemovalInfo;

/**
 * @class UTurnBasedGameplayEffectComponent
 * @brief A component designed for managing turn-based gameplay effects within the Pokémon Battle system.
 *
 * This component extends the gameplay effect functionality to accommodate effects that last for a specified number of turns.
 * It interacts with the UTurnBasedEffectComponent to track and manage the turn-based effects.
 */
UCLASS(DisplayName = "Turn Based")
class POKEMONBATTLE_API UTurnBasedGameplayEffectComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

public:
    bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &GEContainer,
                                     FActiveGameplayEffect &ActiveGE) const override;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;
#endif
    
private:
    static void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo &GERemovalInfo, UTurnBasedEffectComponent* Component);

    /**
     * @brief Specifies the range of turns a gameplay effect should last in the Pokémon Battle system.
     *
     * This variable is optional and represents a range within which the duration of a turn-based gameplay effect falls.
     * If set, a random value within this range will be used to determine the effect's duration in terms of game turns.
     */
    UPROPERTY(EditAnywhere)
    TOptional<FInt32Range> TurnDuration;

};
