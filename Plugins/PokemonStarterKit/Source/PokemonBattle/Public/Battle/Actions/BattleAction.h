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
    virtual const TScriptInterface<IBattler>& GetBattler() const = 0;

};