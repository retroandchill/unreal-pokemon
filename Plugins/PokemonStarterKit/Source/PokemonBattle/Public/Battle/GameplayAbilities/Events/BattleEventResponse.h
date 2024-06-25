// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleEvent.h"
#include "GameplayEffectTypes.h"
#include "UObject/Interface.h"
#include "BattleEventResponse.generated.h"

USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleEventResponse {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = Execution)
    FEventValue ValueToModify;

    /**
     * Modifier operation to perform
     */
    UPROPERTY(EditDefaultsOnly, Category = Execution)
    TEnumAsByte<EGameplayModOp::Type> ModifierOp;
};