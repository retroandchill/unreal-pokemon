// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffect.h"
#include "StatusEffectGameplayEffectComponent.generated.h"

class IBattler;

/**
 * Gameplay effect component that governs the application of status conditions to a battler
 */
UCLASS(DisplayName = "Status Effect")
class POKEMONBATTLE_API UStatusEffectGameplayEffectComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

public:
    bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer &ActiveGEContainer, const FGameplayEffectSpec &GESpec) const override;
    bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& GEContainer, FActiveGameplayEffect& ActiveGE) const override;

private:
    /**
     * Called when the gameplay effect is removed
     * @param GERemovalInfo The remove info for the gameplay effect
     * @param Battler The battler to apply the effect to
     */
    void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& GERemovalInfo, TScriptInterface<IBattler> Battler) const;

    /**
     * The ID of the status effect that this effect represents
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.StatusEffectHelper.GetStatusEffectNames"))
    FName StatusEffectID;

};
