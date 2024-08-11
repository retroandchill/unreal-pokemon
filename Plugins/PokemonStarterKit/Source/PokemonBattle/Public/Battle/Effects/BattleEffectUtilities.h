// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BattleEffectUtilities.generated.h"

struct FGameplayTag;
class IBattler;
/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UBattleEffectUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Apply the specified item effect to the target with the specified magnitude.
     * @param Target The target of the move in question.
     * @param EffectClass The class for the gameplay effect to apply.
     * @param DataTag The gameplay tag for the SetByCaller magnitude value
     * @param Magnitude The value of the magnitude
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Effects", meta = (AutoCreateRefTerm = "EffectClass,DataTag"))
    static void ApplyEffectToTargetWithMagnitude(const TScriptInterface<IBattler>& Target, const TSoftClassPtr<UGameplayEffect>& EffectClass, const FGameplayTag& DataTag, float Magnitude);
    
};
