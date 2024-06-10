// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Battle.generated.h"

class IBattleAction;
class IAbilityBattleEffect;
class IFieldEffect;
class IBattler;

// This class does not need to be modified.
UINTERFACE()
class POKEMONBATTLE_API UBattle : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents an active battle that is occurring.
 */
class POKEMONBATTLE_API IBattle {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * This is to be called after all pre-battle setup has been completed (i.e. intro animations, sending out Pokémon,
     * etc.)
     */
    virtual void StartBattle() = 0;

    virtual void QueueAction(const TScriptInterface<IBattleAction>& Action) = 0;

    virtual bool ActionSelectionFinished() const = 0;
    
    /**
     * Should this battle actively ignore abilities
     * @return Should abilities be ignored
     */
    virtual bool ShouldIgnoreAbilities() const = 0;

    /**
     * Perform a sweep over each of the battlers and perform a callback on each one
     * @param Callback The callback to perform
     */
    virtual void ForEachActiveBattler(const TFunctionRef<void(const TScriptInterface<IBattler>&)>& Callback) const = 0;

    /**
     * Perform a sweep over each of the active field effects and perform a callback on each one
     * @param Callback The callback to perform
     */
    virtual void ForEachFieldEffect(const TFunctionRef<void(const TScriptInterface<IFieldEffect>&)> Callback) const = 0;

    /**
     * Check to see if there is a Pokémon on the field with a given ability that is not actively supressed.
     * @param AbilityID The ID of the ability in question
     * @return Was the ability found
     */
    virtual bool FindGlobalAbility(FName AbilityID) const = 0;

};