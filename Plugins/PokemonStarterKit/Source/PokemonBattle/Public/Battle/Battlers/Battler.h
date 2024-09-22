// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "Battle/Status.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Ranges/Views/AnyView.h"
#include "UObject/Interface.h"
#include "Utilities/Node/Utility_ProcessLevelUp.h"

#include "Battler.generated.h"

class IBattler;
struct FSpeciesData;
class UBattlerAbilityComponent;
class IPokemon;
class IBattleSide;
class IBattleMove;
class IBattlerController;

DECLARE_DYNAMIC_DELEGATE(FBattleLevelUpEnd);

/**
 * Information about a status effect inflicted upon the battler
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FStatusEffectInfo {
    GENERATED_BODY()

    /**
     * The ID of the inflicted status effect
     */
    UPROPERTY(BlueprintReadOnly, Category = StatusEffects)
    FName StatusEffectID;

    /**
     * The gameplay effect that represents the status effect
     */
    UPROPERTY(BlueprintReadOnly, Category = StatusEffects)
    FActiveGameplayEffectHandle EffectHandle;

    /**
     * Default constructor
     */
    FStatusEffectInfo() = default;

    /**
     * Create a new effect object with the provided arguments
     * @param StatusEffectID The ID of the inflicted status effect
     * @param EffectHandle The gameplay effect that represents the status effect
     */
    FStatusEffectInfo(FName StatusEffectID, FActiveGameplayEffectHandle EffectHandle);
};

/**
 * Information about Exp. that the battler gained
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FExpGainInfo {
    GENERATED_BODY()

    /**
     * The battler who is gaining Exp.
     */
    UPROPERTY(BlueprintReadOnly, Category = Exp)
    TScriptInterface<IBattler> GainingBattler;

    /**
     * The amount of Exp. that is gained for display.
     */
    UPROPERTY(BlueprintReadOnly, Category = Exp)
    int32 Amount = 0;

    UPROPERTY(BlueprintReadOnly, Category = Exp)
    FLevelUpStatChanges StatChanges;

    FExpGainInfo() = default;

    FExpGainInfo(const TScriptInterface<IBattler> &Battler, int32 Amount);
};

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
    UFUNCTION(BlueprintCallable, Category = Context)
    virtual const TScriptInterface<IBattleSide> &GetOwningSide() const = 0;

    UFUNCTION(BlueprintCallable, Category = Context)
    virtual const TScriptInterface<IPokemon> &GetWrappedPokemon() const = 0;

    UFUNCTION(BlueprintCallable, Category = Context)
    virtual bool IsActive() const = 0;

    /**
     * Get the species that this battler represents
     * @return The species of this battler
     */
    virtual const FSpeciesData &GetSpecies() const = 0;

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

    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual void RefreshStats() = 0;

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
     * Check to see if the battler in question has fainted
     * @return Has the Pokémon fainted
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual bool IsNotFainted() const = 0;

    /**
     * Visually faint the battler
     */
    UFUNCTION(BlueprintCallable, Category = Visuals)
    virtual void Faint() const = 0;

    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual bool CanGainExp() const = 0;

    /**
     * Get the percent value of Exp to a level up
     * @return The Pokémon's exp to level up percentage
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual float GetExpPercent() const = 0;

    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual TArray<FExpGainInfo> GiveExpToParticipants() = 0;

    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FLevelUpStatChanges GainExpAndEVs(int32 Exp, const TMap<FName, uint8> &EVs) = 0;

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
    UFUNCTION(BlueprintCallable, Category = GameplayAbilities)
    virtual UBattlerAbilityComponent *GetAbilityComponent() const = 0;

    /**
     * Get the Pokémon's currently usable moves.
     * @return The Pokémon's usable moves.
     */
    UFUNCTION(BlueprintCallable, Category = Moves)
    virtual const TArray<TScriptInterface<IBattleMove>> &GetMoves() const = 0;

    UFUNCTION(BlueprintCallable, Category = Switching)
    virtual FText GetRecallMessage() const = 0;

    virtual FGameplayAbilitySpecHandle PerformSwitch(const TScriptInterface<IBattler> &SwitchTarget) = 0;

    UFUNCTION(BlueprintCallable, Category = Ownership)
    virtual bool IsOwnedByPlayer() const = 0;

    /**
     * Select the actions for this battler
     */
    virtual void SelectActions() = 0;

    /**
     * Force the battler to choose a party member to switch to
     */
    virtual void RequireSwitch() = 0;

    /**
     * How many actions a Pokémon may take in a turn.
     * <p>Note: Messages like "Pikachu cannot move" or "Snorlax must recharge" count as actions</p>
     * @return The number of actions that can be taken
     */
    virtual uint8 GetActionCount() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Battle|TurnFlow")
    virtual int32 GetTurnCount() const = 0;

    /**
     * Get all allies in battle
     * @return A view of all allies
     */
    virtual UE::Ranges::TAnyView<TScriptInterface<IBattler>> GetAllies() const = 0;

    /**
     * Show the battler's sprite in battle
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Visuals, meta = (AutoCreateRefTerm = Offset))
#if CPP
    virtual void ShowSprite(const FVector& Offset = FVector()) const = 0;
#else
    virtual void ShowSprite(const FVector& Offset) const = 0;
#endif

    /**
     * Hide the battler's sprite
     */
    UFUNCTION(BlueprintCallable, Category = Visuals)
    virtual void HideSprite() const = 0;

    virtual void RecordParticipation() = 0;

    virtual void AddParticipant(const TScriptInterface<IBattler> &Participant) = 0;

    /**
     * Get the status effect held by this battler
     * @return The status effect held by the battler
     */
    virtual const TOptional<FStatusEffectInfo> &GetStatusEffect() const = 0;

    /**
     * Inflict a status effect onto the battler
     * @param StatusEffectID The ID of the status effect to inflict
     * @param EffectHandle The handle for the corresponding gameplay effect
     */
    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    virtual void InflictStatusEffect(FStatusHandle StatusEffectID, FActiveGameplayEffectHandle EffectHandle) = 0;

    /**
     * Remove the battler's status effect
     */
    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    virtual void CureStatusEffect() = 0;
};