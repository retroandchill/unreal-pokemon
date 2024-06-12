// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleSide.generated.h"

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
    virtual TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                                     const TScriptInterface<IPokemon> &Pokemon, bool ShowBackSprites = false) = 0;

    /**
     * Create a side containing a single-wild Pokémon
     * @param Battle The battle to take ownership of this side
     * @param Trainer The trainer to battle against
     * @param PokemonCount
     * @param ShowBackSprites
     * @return This side post-initialization
     */
    virtual TScriptInterface<IBattleSide> Initialize(const TScriptInterface<IBattle> &Battle,
                                                     const TScriptInterface<ITrainer> &Trainer, uint8 PokemonCount = 1, bool ShowBackSprites = false) = 0;

    virtual TScriptInterface<IBattle> GetOwningBattle() const = 0;

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

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    virtual bool ShowBackSprites() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Battle|Visuals")
    virtual void SendOutBattlers() const = 0;
    
    virtual const TArray<TScriptInterface<IBattler>>& GetBattlers() const = 0;

};