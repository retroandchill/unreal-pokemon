// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PartyScreen.generated.h"

class UPartyMenuHandler;
class ITrainer;

/**
 * Delegate for when an item is selected
 */
DECLARE_DELEGATE_ThreeParams(FOnPokemonSelected, const TScriptInterface<class IPartyScreen> &,
                             const TScriptInterface<ITrainer> &, int32);

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONUI_API UPartyScreen : public UInterface {
    GENERATED_BODY()
};

/**
 * Generic screen class for any screens shown to the player
 */
class POKEMONUI_API IPartyScreen {
    GENERATED_BODY()

  public:
    /**
     * Begin the switching process.
     * @param Index The index to start the switch at.
     */
    UFUNCTION(BlueprintCallable, Category = Switching)
    virtual void BeginSwitch(int32 Index) = 0;

    /**
     * Show a list of commands taking in the provided handlers to generate the list.
     * @param Handlers The handlers to take in for the party menu
     */
    virtual void ShowCommands(const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers) = 0;

    /**
     * Clear out the stack of commands reverting back to how things normally look
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void ClearCommandStack() = 0;

    /**
     * Set the text of the help window
     * @param Text The new text for the help window
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void SetCommandHelpText(FText Text) = 0;

    /**
     * Get the player controller held by this class
     * @return The player controller currently held
     */
    UFUNCTION(BlueprintCallable, Category = Owner)
    virtual APlayerController *GetPlayerController() const = 0;

    /**
     * Get the callback override for when a Pokémon is selected
     * @return Override to the Pokémon selection callback
     */
    virtual FOnPokemonSelected &GetOnPokemonSelect() = 0;

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
