// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Actions/BattleActionUseItem.h"
#include "UObject/Object.h"
#include "UseItemPayload.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UUseItemPayload : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY()
    FName Item;

    UPROPERTY()
    TObjectPtr<const UGameplayAbility> Ability;

};
