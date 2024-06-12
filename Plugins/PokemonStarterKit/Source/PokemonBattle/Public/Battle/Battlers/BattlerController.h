// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattlerController.generated.h"

class IBattler;
class IBattleAction;

DECLARE_DELEGATE_OneParam(FActionReady, TUniquePtr<IBattleAction>&&);

// This class does not need to be modified.
UINTERFACE()
class UBattlerController : public UInterface {
    GENERATED_BODY()
};

/**
 * The base controller interface for handling how battlers select their actions.
 */
class POKEMONBATTLE_API IBattlerController {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void InitiateActionSelection(const TScriptInterface<IBattler>& Battler) const = 0; 

    virtual void BindOnActionReady(FActionReady&& QueueAction) = 0;;
};