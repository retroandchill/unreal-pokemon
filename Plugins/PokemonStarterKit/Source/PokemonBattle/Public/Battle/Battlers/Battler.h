// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "range/v3/view/any_view.hpp"
#include "BattlerStats.h"
#include "UObject/Interface.h"

#include "Battler.generated.h"

class UBattlerAbilityComponent;
class IPokemon;
class IBattleSide;
class IBattleMove;
class IBattlerEffect;
class IHoldItemBattleEffect;
class IBattlerController;

    // This class does not need to be modified.
    UINTERFACE(NotBlueprintable, BlueprintType) class POKEMONBATTLE_API UBattler : public UInterface {
    GENERATED_BODY()
};

/**
 * Wrapper around a Pokémon for its usage in battle
 */
class POKEMONBATTLE_API IBattler {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Create this battler with the given parameters
     * @param Side The side that owns this battler
     * @param Pokemon The Pokémon this wraps around
     * @param ShowImmediately Whether or not this battler should be visible immediately upon being spawned
     * @return
     */
    virtual TScriptInterface<IBattler> Initialize(const TScriptInterface<IBattleSide> &Side,
                                                  const TScriptInterface<IPokemon> &Pokemon = nullptr,
                                                  bool ShowImmediately = false) = 0;

    /**
     * Get the internal ID for this battler
     * @return The unique ID of the battler
     */
    UFUNCTION(BlueprintCallable, Category = Metadata)
    virtual FGuid GetInternalId() const = 0;

    /**
     * Get the side that this battler is a part of
     * @return The battler's side
     */
    virtual const TScriptInterface<IBattleSide> &GetOwningSide() const = 0;

    /**
     * Get the battler's display name
     * @return The display name of the battler
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FText GetNickname() const = 0;

    /**
     * Get the Pokémon's gender
     * @return The Pokémon's gender
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual EPokemonGender GetGender() const = 0;

    /**
     * Get the Pokémon's level
     * @return The level of the Pokémon in question
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetPokemonLevel() const = 0;

    /**
     * Get the value of the Pokémon's current HP
     * @return The Pokémon's current HP
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetHP() const = 0;

    /**
     * Get the value of the Pokémon's Max HP.
     * @return The Pokémon's Max HP
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetMaxHP() const = 0;

    /**
     * Get the value of the Pokémon's Max HP.
     * @return The Pokémon's Max HP
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Get HP Percent", Category = Stats)
    virtual float GetHPPercent() const = 0;

    /**
     * Take damage and apply that change to the Pokémon's current HP
     * @param Damage The damage that was taken
     */
    virtual void TakeBattleDamage(int32 Damage) = 0;

    /**
     * Check to see if the battler in question has fainted
     * @return Has the Pokémon fainted
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual bool IsFainted() const = 0;

    /**
     * Visually faint the battler
     */
    UFUNCTION(BlueprintCallable, Category = Visuals)
    virtual void Faint() const = 0;

    /**
     * Get the value of the Pokémon's physical attack.
     * @return The Pokémon's physical attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FMainBattleStat GetAttack() const = 0;

    /**
     * Get the value of the Pokémon's physical defense.
     * @return The Pokémon's physical defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FMainBattleStat GetDefense() const = 0;

    /**
     * Get the value of the Pokémon's special attack.
     * @return The Pokémon's special attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FMainBattleStat GetSpecialAttack() const = 0;

    /**
     * Get the value of the Pokémon's special defense.
     * @return The Pokémon's special defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FMainBattleStat GetSpecialDefense() const = 0;

    /**
     * Get the value of the Pokémon's speed
     * @return The Pokémon's speed
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FMainBattleStat GetSpeed() const = 0;

    /**
     * Get the stat stage for the given value
     * @param Stat The stat to look up
     * @return The number of stages in the given stat
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetStatStage(FName Stat) const = 0;

    /**
     * Get the percent value of Exp to a level up
     * @return The Pokémon's exp to level up percentage
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual float GetExpPercent() const = 0;

    /**
     * Get the Pokémon's current type
     * @return The type of the Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual TArray<FName> GetTypes() const = 0;

    /**
     * Get the gameplay ability component used by this battler
     * @return The held battler ability component
     */
    virtual UBattlerAbilityComponent* GetAbilityComponent() const = 0;

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
    virtual const TScriptInterface<IHoldItemBattleEffect> &GetHoldItem() const = 0;

    /**
     * Get the Pokémon's currently usable moves.
     * @return The Pokémon's usable moves.
     */
    UFUNCTION(BlueprintCallable, Category = Moves)
    virtual const TArray<TScriptInterface<IBattleMove>> &GetMoves() const = 0;

    /**
     * Select the actions for this battler
     */
    virtual void SelectActions() = 0;

    /**
     * How many actions a Pokémon may take in a turn.
     * <p>Note: Messages like "Pikachu cannot move" or "Snorlax must recharge" count as actions</p>
     * @return The number of actions that can be taken
     */
    virtual uint8 GetActionCount() const = 0;

    /**
     * Get all allies in battle
     * @return A view of all allies
     */
    virtual ranges::any_view<TScriptInterface<IBattler>> GetAllies() const = 0;

    /**
     * Show the battler's sprite in battle
     */
    virtual void ShowSprite() const = 0;
};