// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleDamage.h"
#include "UObject/Object.h"
#include "MoveEffectContext.generated.h"

/**
 * The context for the effect of the move
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UMoveEffectContext : public UObject {
    GENERATED_BODY()

public:
    /**
     * Get the damage info for the move being used
     * @return The damage info for move being used
     */
    UFUNCTION(BlueprintPure, Category = "Moves|Damage")
    const FMoveDamageInfo& GetDamageInfo() const;
    
    /**
     * Set the new damage info for this context object
     * @param NewDamageInfo The context for move being used
     */
    void SetDamageInfo(const TSharedRef<FMoveDamageInfo>& NewDamageInfo);

private:
    /**
     * The context of the move in question
     */
    TSharedPtr<FMoveDamageInfo> DamageInfo;

};
