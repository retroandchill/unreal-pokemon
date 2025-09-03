// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Battle/Effects/TurnBasedGameplayEffect.h"
#include "Components/ActorComponent.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

#include "TurnBasedEffectComponent.generated.h"

struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;

/**
 * UTurnBasedEffectComponent is a component class that manages turn-based gameplay effects in a turn-based game.
 * It inherits from UActorComponent and integrates with the AbilitySystemComponent to apply and manage effects based on
 * game turns.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POKEMONBATTLE_API UTurnBasedEffectComponent : public UActorComponent {
    GENERATED_BODY()

  public:
    void BeginPlay() override;

    /**
     * Gets the AbilitySystemComponent associated with this component.
     *
     * @return A pointer to the AbilitySystemComponent.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UAbilitySystemComponent *GetAbilitySystemComponent() const {
        return AbilitySystemComponent;
    }

    /**
     * Adds a turn-based gameplay effect to the component, tracking its duration in terms of game turns.
     *
     * @param Handle The handle to the active gameplay effect to be added.
     * @param TurnDuration The number of turns the effect should last.
     */
    void AddTurnBasedEffect(FActiveGameplayEffectHandle Handle, int32 TurnDuration = INDEX_NONE);

    /**
     * Removes a turn-based gameplay effect from the component using the specified handle.
     *
     * @param Handle The handle to the active gameplay effect that should be removed.
     * @return True if the gameplay effect was successfully removed; otherwise false.
     */
    bool RemoveTurnBasedEffect(FActiveGameplayEffectHandle Handle);

    /**
     * Retrieves all active turn-based gameplay effects that correspond to a specified trigger.
     *
     * @param Trigger The specific turn duration trigger to filter effects by.
     * @return A collection of active gameplay effects filtered to include only those with the specified trigger.
     */
    auto GetAllTurnBasedEffectsForTrigger(ETurnDurationTrigger Trigger) {
        // clang-format off
        return ActiveTurnBasedEffects |
               Retro::Ranges::Views::Values |
               Retro::Ranges::Views::Filter(Retro::BindBack<&FTurnBasedGameplayEffect::HasTrigger>(Trigger));
        // clang-format on
    }

  private:
    /**
     * AbilitySystemComponent is a reference to the UAbilitySystemComponent associated with this component.
     * It is used to apply and manage gameplay abilities and effects.
     * This variable integrates with the GameplayAbilities system, offering a way to coordinate ability interactions.
     */
    UPROPERTY(BlueprintGetter = GetAbilitySystemComponent, Category = GameplayAbilities)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TMap<FActiveGameplayEffectHandle, FTurnBasedGameplayEffect> ActiveTurnBasedEffects;
};