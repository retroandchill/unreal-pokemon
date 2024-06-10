// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleAction.generated.h"

class IBattler;
// This class does not need to be modified.
UINTERFACE()
class POKEMONBATTLE_API UBattleAction : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents an action to be taken in battle.
 */
class POKEMONBATTLE_API IBattleAction {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual const TScriptInterface<IBattler>& GetBattler() const = 0;

};