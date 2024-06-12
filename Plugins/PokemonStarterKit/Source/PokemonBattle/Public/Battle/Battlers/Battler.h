// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Battler.generated.h"

class IPokemon;
class IBattleSide;
class IBattleMove;
class IBattlerEffect;
class IHoldItemBattleEffect;
class IAbilityBattleEffect;
class IMoveModifier;
// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONBATTLE_API UBattler : public UInterface {
    GENERATED_BODY()
};

/**
 * Wrapper around a Pokémon for its usage in battle
 */
class POKEMONBATTLE_API IBattler {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual TScriptInterface<IBattler> Initialize(const TScriptInterface<IBattleSide> &Side, const TScriptInterface<IPokemon>& Pokemon = nullptr, bool ShowImmediately = false) = 0;
    
    virtual FGuid GetInternalId() const = 0;

    virtual TScriptInterface<IBattleSide> GetOwningSide() const = 0;

    virtual FText GetNickname() const = 0;
    
    /**
     * Get the Pokémon's level
     * @return The level of the Pokémon in question
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetPokemonLevel() const = 0;

    /**
     * Get the value of the Pokémon's physical attack.
     * @return The Pokémon's physical attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetAttack() const = 0;

    /**
     * Get the value of the Pokémon's physical defense.
     * @return The Pokémon's physical defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetDefense() const = 0;

    /**
     * Get the value of the Pokémon's special attack.
     * @return The Pokémon's special attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetSpecialAttack() const = 0;

    /**
     * Get the value of the Pokémon's special defense.
     * @return The Pokémon's special defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetSpecialDefense() const = 0;

    /**
     * Get the Pokémon's current type
     * @return The type of the Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual TArray<FName> GetTypes() const = 0;

    /**
     * Get if the target's current ability is active
     * @return Is the target's ability active?
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual bool IsAbilityActive() const = 0;

    /**
     * Get the Pokémon's current ability effect
     * @return The effect of the ability in question
     */
    UFUNCTION(BlueprintCallable, Category = Items)
    virtual const TScriptInterface<IAbilityBattleEffect>& GetAbility() const = 0;

    /**
     * Get if the target's current hold item is active
     * @return Is the target's hold item active?
     */
    UFUNCTION(BlueprintCallable, Category = Items)
    virtual bool IsHoldItemActive() const = 0;

    /**
     * Get the Pokémon's current hold item effect
     * @return The effect of the hold item in question
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual const TScriptInterface<IHoldItemBattleEffect>& GetHoldItem() const = 0;

    /**
     * Get the Pokémon's currently usable moves.
     * @return The Pokémon's usable moves.
     */
    UFUNCTION(BlueprintCallable, Category = Moves)
    virtual const TArray<TScriptInterface<IBattleMove>>& GetMoves() const = 0;

    /**
     * Select the actions for this battler
     */
    virtual void SelectActions() const = 0;

    /**
     * How many actions a Pokémon may take in a turn.
     * <p>Note: Messages like "Pikachu cannot move" or "Snorlax must recharge" count as actions</p>
     * @return The number of actions that can be taken 
     */
    virtual uint8 GetActionCount() const = 0;

    /**
     * Iterate over all of the battler's allies and apply the callback to them
     * @param Callback The callback to apply to the battlers
     */
    virtual void ForEachAlly(const TFunctionRef<void(const TScriptInterface<IBattler>&)>& Callback) const = 0;

    /**
     * Iterate over each battle effect active on the user and apply said effect to the callback
     * @param Callback The callback to run on each effect
     */
    virtual void ForEachBattleEffect(const TFunctionRef<void(const TScriptInterface<IBattlerEffect>&)>& Callback) const = 0;

};