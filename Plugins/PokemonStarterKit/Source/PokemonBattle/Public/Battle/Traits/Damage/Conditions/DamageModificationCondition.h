// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "DamageModificationCondition.generated.h"

struct FMoveDamageInfo;
struct FDamageMultipliers;

/**
 * Condition type used for applying damage modification
 */
UCLASS(Abstract, EditInlineNew)
class POKEMONBATTLE_API UDamageModificationCondition : public UObject {
    GENERATED_BODY()

  public:
    /**
     * Evaluate the move damage context and validate if the condition is true
     * @param Context The context for the move udage
     * @return Should the effect be applied
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    bool Evaluate(const FMoveDamageInfo &Context) const;
};
