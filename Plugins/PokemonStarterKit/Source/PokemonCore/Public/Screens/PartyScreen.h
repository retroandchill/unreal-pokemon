// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "PartyScreen.generated.h"

class IPartyScreen;
class IPokemon;
class UPartyMenuHandler;
class ITrainer;

struct POKEMONCORE_API FSelectedPokemonHandle {
    FSelectedPokemonHandle(const TScriptInterface<IPartyScreen> &InScreen, const TScriptInterface<ITrainer> &InTrainer,
                           int32 InIndex);

    const TScriptInterface<IPartyScreen> &GetScreen() const {
        return Screen;
    }

    const TScriptInterface<ITrainer> &GetTrainer() const {
        return Trainer;
    }

    int32 GetIndex() const {
        return Index;
    }

    TScriptInterface<IPokemon> GetPokemon() const;

  private:
    TScriptInterface<IPartyScreen> Screen;
    TScriptInterface<ITrainer> Trainer;
    int32 Index;
};

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONCORE_API UPartyScreen : public UInterface {
    GENERATED_BODY()
};

/**
 * Generic screen class for any screens shown to the player
 */
class POKEMONCORE_API IPartyScreen {
    GENERATED_BODY()

  public:
    /**
     * Begin the switching process.
     * @param Index The index to start the switch at.
     */
    UFUNCTION(BlueprintCallable, Category = Switching)
    virtual void BeginSwitch(int32 Index) = 0;

    /**
     * Get the player controller held by this class
     * @return The player controller currently held
     */
    UFUNCTION(BlueprintCallable, Category = Owner)
    virtual APlayerController *GetPlayerController() const = 0;

    virtual UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>> PromptPokemonSelection() = 0;

    /**
     * Remove the screen from the stack
     */
    UFUNCTION(BlueprintCallable, Category = Navigation)
    virtual void RemoveFromStack() = 0;

    /**
     * Refresh the display of the scene
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void RefreshScene() = 0;
};
