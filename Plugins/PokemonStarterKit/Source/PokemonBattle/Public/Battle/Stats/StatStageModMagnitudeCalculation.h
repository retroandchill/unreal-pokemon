// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Species/PokemonStatType.h"

#include "StatStageModMagnitudeCalculation.generated.h"

/**
 * Modifier calculation for a stat stage
 */
UCLASS(Abstract, Blueprintable, HideCategories = "Attributes")
class POKEMONBATTLE_API UStatStageModMagnitudeCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    /**
     * The ID of the stat in question
     */
    UPROPERTY(EditDefaultsOnly, Category = "Battle|Stats",
        meta = (GetOptions = "PokemonData.StatHelper.GetMainBattleStatNames"))
    FName StatID;
    
    /**
     * The captured attribute for the stages.
     */
    UPROPERTY(VisibleDefaultsOnly, Category = "Battle|Stats")
    FGameplayEffectAttributeCaptureDefinition StagesAttribute;
};
