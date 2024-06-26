// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActionResult.h"
#include "UObject/Interface.h"

class IBattler;

/**
 * Represents an action to be taken in battle.
 */
class POKEMONBATTLE_API IBattleAction {

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual ~IBattleAction() = default;

    /**
     * Get the battler that owns the given battle action
     * @return The owning battler
     */
    virtual const TScriptInterface<IBattler> &GetBattler() const = 0;

    /**
     * Can this action actually be executed?
     * @return Is the action still usable
     */
    virtual bool CanExecute() const = 0;

    /**
     * Execute the action in question
     * @param bPerformAsync Should action computation be performed asynchronously
     */
    virtual void Execute(bool bPerformAsync = true) = 0;

    /**
     * Is this action currently executing?
     * @return Is the action currently executing?
     */
    virtual bool IsExecuting() const = 0;

    /**
     * Get the action's priority in battle
     * @return The priority of the action
     */
    virtual int32 GetPriority() const = 0;

    /**
     * Get the usage message to display for the action
     * @return The usage message to display for the given action
     */
    virtual FText GetActionMessage() const = 0;

    /**
     * Get the pending result of the action once it is complete
     * @return The result of the action once calculation has been completed
     */
    virtual const TFuture<FActionResult> &GetActionResult() const = 0;
};