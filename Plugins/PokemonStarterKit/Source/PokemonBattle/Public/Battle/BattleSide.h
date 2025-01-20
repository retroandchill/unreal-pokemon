// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Ranges/Views/AnyView.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "BattleSide.generated.h"

class UTurnBasedEffectComponent;
class UAbilitySystemComponent;
class IPokemon;
class ITrainer;
struct FPokemonDTO;
class IBattle;
class IBattler;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONBATTLE_API UBattleSide : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents a "side" of the battle.
 */
class POKEMONBATTLE_API IBattleSide {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Create a side containing a single-wild Pokémon
     * @param Battle The battle to take ownership of this side
     * @param Pokemon The Pokémon to battle against
     * @param ShowBackSprites
     * @return This side post-initialization
     */
    virtual UE5Coro::TCoroutine<TScriptInterface<IBattleSide>> Initialize(TScriptInterface<IBattle> Battle,
                                                                          TArray<TScriptInterface<IPokemon>> Pokemon,
                                                                          bool ShowBackSprites = false) = 0;

    /**
     * Create a side containing a single-wild Pokémon
     * @param Battle The battle to take ownership of this side
     * @param Trainer The trainer to battle against
     * @param PokemonCount
     * @param ShowBackSprites
     * @return This side post-initialization
     */
    virtual UE5Coro::TCoroutine<TScriptInterface<IBattleSide>> Initialize(TScriptInterface<IBattle> Battle,
                                                                          TScriptInterface<ITrainer> Trainer,
                                                                          uint8 PokemonCount = 1,
                                                                          bool ShowBackSprites = false) = 0;

    virtual const FGuid &GetInternalId() const = 0;

    /**
     * Get the battle that owns this particular side
     * @return The battle context for this side of combat
     */
    virtual const TScriptInterface<IBattle> &GetOwningBattle() const = 0;

    /**
     * Retrieve the turn-based effect component associated with this battle side.
     * This component handles effects that span over multiple turns such as status conditions,
     * stat changes, and other ongoing effects in the battle.
     * @return The turn-based effect component for this side.
     */
    UFUNCTION(BlueprintCallable, Category = GameplayAbilities)
    virtual UTurnBasedEffectComponent *GetTurnBasedEffectComponent() const = 0;

    virtual Retro::TGenerator<UTurnBasedEffectComponent *> GetChildEffectComponents() const = 0;

    /**
     * The capacity for battlers on a given side.
     * @return The total number of battlers on the side
     */
    virtual uint8 GetSideSize() const = 0;

    /**
     * Get the intro text for the side to display to the player.
     * @return The intro text in question
     */
    virtual const FText &GetIntroText() const = 0;

    /**
     * Get the text that corresponds to Pokémon being sent out. This is skipped over if the optional is empty.
     * @return The text to display on a Pokémon send out
     */
    virtual const TOptional<FText> &GetSendOutText() const = 0;

    /**
     * Is this side positioned to where the backsprites should be shown
     * @return Should we show backsprites
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    virtual bool ShowBackSprites() const = 0;

    /**
     * Make any hidden battler sprites visible. This should be called from the point in the animation where the Pokémon
     * are shown emerging from their Poké Balls.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    virtual void SendOutBattlers() const = 0;

    /**
     * Get the list of battlers for this side
     * @return The battlers for this side.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Battlers")
    virtual const TArray<TScriptInterface<IBattler>> &GetBattlers() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Battle|Battlers")
    virtual const TArray<TScriptInterface<ITrainer>> &GetTrainers() const = 0;

    virtual const TArray<TScriptInterface<IBattler>> &
    GetTrainerParty(const TScriptInterface<ITrainer> &Trainer) const = 0;

    virtual void SwapBattlerPositions(const TScriptInterface<ITrainer> &Trainer, int32 IndexA, int32 IndexB) = 0;

    /**
     * Determine if the side can still fight or not
     * @return Can this side fight
     */
    virtual bool CanBattle() const = 0;
};