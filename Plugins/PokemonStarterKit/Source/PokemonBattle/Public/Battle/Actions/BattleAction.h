// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
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
     */
    virtual void Execute() = 0;

    /**
     * Is this action currently executing?
     * @return Is the action currently executing?
     */
    virtual bool IsExecuting() const = 0;

    /**
     * Is this action's execution complete?
     * @return Is the action's execution complete?
     */
    virtual bool IsComplete() const = 0;

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
};