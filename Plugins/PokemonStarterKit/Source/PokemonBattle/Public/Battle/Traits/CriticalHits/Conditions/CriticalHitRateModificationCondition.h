// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CriticalHitRateModificationCondition.generated.h"

class IBattler;

/**
 * The condition for the modification of critical hit rates by a trait.
 */
UCLASS(Abstract, Abstract, EditInlineNew)
class POKEMONBATTLE_API UCriticalHitRateModificationCondition : public UObject {
    GENERATED_BODY()

public:
    /**
     * Evaluate the move damage context and validate if the condition is true
     * @param User The user of the move
     * @param Target The target of the move
     * @return Should the effect be applied
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    bool Evaluate(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) const;

};
