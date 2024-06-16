// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Functional/FunctionalShorthands.h"
#include "UObject/Interface.h"
#include <range/v3/view/any_view.hpp>

#include "Battle.generated.h"

class ITraitHolder;
class IBattleSide;
class IBattleAction;
class IAbilityBattleEffect;
class IFieldEffect;
class IBattler;

using FSideWithIndexCallback = const TFunctionRef<void(int32, const TScriptInterface<IBattleSide> &)> &;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
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
    virtual TScriptInterface<IBattle> Initialize(TArray<TScriptInterface<IBattleSide>> &&SidesIn) = 0;

    /**
     * Have the player take possession of the battle pawn and begin the battle intro.
     * @param PlayerController The player controller to shift control over to the battle pawn
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Flow")
    void JumpToBattleScene(APlayerController *PlayerController);

    /**
     * This is to be called after all pre-battle setup has been completed (i.e. intro animations, sending out Pokémon,
     * etc.)
     */
    virtual void StartBattle() = 0;

    /**
     * Add an action to the pending queue
     * @param Action The action to take
     */
    virtual void QueueAction(TUniquePtr<IBattleAction> &&Action) = 0;

    /**
     * Check if action selection is completed
     * @return Is action selection done
     */
    virtual bool ActionSelectionFinished() const = 0;

    /**
     * Should this battle actively ignore abilities
     * @return Should abilities be ignored
     */
    virtual bool ShouldIgnoreAbilities() const = 0;

    /**
     * Get the battle pawn used in battle
     * @return The pawn used in battle
     */
    virtual APawn *GetBattlePawn() const = 0;

    /**
     * Iterate over each side of the battle and perform the given callback on said side
     * @param Callback The callback to execute on each iteration
     */
    virtual void ForEachSide(FSideWithIndexCallback Callback) const = 0;

    /**
     * Perform a sweep over each of the battlers and perform a callback on each one
     * @param Callback The callback to perform
     */
    virtual void ForEachActiveBattler(TInterfaceCallback<IBattler> Callback) const = 0;

    /**
     * Perform a sweep over each of the active field effects and perform a callback on each one
     * @param Callback The callback to perform
     */
    virtual void ForEachFieldEffect(TInterfaceCallback<IFieldEffect> Callback) const = 0;
    
    virtual bool AnyTraitHolder(const TFunctionRef<bool(const ITraitHolder &)>& Predicate) const = 0;

    virtual ranges::any_view<ITraitHolder * const&> GetTraitHolders() const = 0;


    /**
     * Check to see if there is a Pokémon on the field with a given ability that is not actively supressed.
     * @param AbilityID The ID of the ability in question
     * @return Was the ability found
     */
    virtual bool FindGlobalAbility(FName AbilityID) const = 0;

    /**
     * Initiate the process of selecting actions for the given battler.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Selection")
    void BeginActionSelection(const TScriptInterface<IBattler> &Battler);

    /**
     * Execute the bound action in battle
     * @param Action The action to execute
     */
    virtual void ExecuteAction(IBattleAction &Action) = 0;
};