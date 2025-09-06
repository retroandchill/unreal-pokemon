// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AsyncAbilityComponent.h"

#include "BattleAbilitySystemComponent.generated.h"

/**
 * The ability system component used by the battle actor.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattleAbilitySystemComponent : public UAsyncAbilityComponent
{
    GENERATED_BODY()
};