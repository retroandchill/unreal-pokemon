// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Actions/BattleAction.h"
#include "UObject/Interface.h"

#include "BattlerController.generated.h"

class IBattler;

DECLARE_DELEGATE_OneParam(FActionReady, TUniquePtr<IBattleAction> &&);

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
    /**
     * Initiate the selection process for this battler
     * @param Battler
     */
    virtual void InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const = 0;

    virtual void InitiateForcedSwitch(const TScriptInterface<IBattler>& Battler) const = 0;

    /**
     * Bind a callback to when an action is ready
     * @param QueueAction The action to take when ready to act
     */
    virtual void BindOnActionReady(FActionReady &&QueueAction) = 0;
};