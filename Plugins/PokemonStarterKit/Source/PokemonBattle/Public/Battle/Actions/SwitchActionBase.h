// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SwitchActionBase.generated.h"

/**
 * Base gameplay ability class for the switch action in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API USwitchActionBase : public UGameplayAbility {
    GENERATED_BODY()

public:
    /**
     * Default constructor.
     */
    USwitchActionBase();

};
