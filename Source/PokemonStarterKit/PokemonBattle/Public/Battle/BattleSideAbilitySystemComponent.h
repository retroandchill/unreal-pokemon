// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AsyncAbilityComponent.h"

#include "BattleSideAbilitySystemComponent.generated.h"

/**
 * The ability system component used by a given side of battle
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POKEMONBATTLE_API UBattleSideAbilitySystemComponent : public UAsyncAbilityComponent {
    GENERATED_BODY()
};