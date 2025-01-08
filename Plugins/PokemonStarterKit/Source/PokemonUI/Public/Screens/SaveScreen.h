// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Async/GameThreadFutureExecutor.h"
#include "RetroLib/Concepts/Delegates.h"
#include "RetroLib/Functional/Delegates.h"
#include "Screens/Screen.h"

#include "SaveScreen.generated.h"

class UEnhancedSaveGame;
class UDisplayText;
class USaveGame;
class UPokemonSaveGame;
class USaveGameCard;

DECLARE_DELEGATE_OneParam(FOnSaveComplete, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FExitSaveScreen, bool);

/**
 * The screen used to save the game.
 */
UCLASS(Abstract)
class POKEMONUI_API USaveScreen : public UScreen {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static USaveScreen *AddSaveScreenToStack(const UObject *WorldContextObject);

  protected:
    void NativeOnActivated() override;

    /**
     * Set the save game being used by this window
     * @param SaveGame The save game being used by the window
     */
    UFUNCTION(BlueprintCallable, Category = Saving)
    void SetSaveGame(UEnhancedSaveGame *SaveGame);

    /**
     * Event triggered when the save game has been set.
     * This can be implemented in Blueprints to define custom behavior that occurs
     * when a new save game is assigned.
     * @param SaveGame The save game object that has been set.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Saving)
    void OnSaveGameSet(UPokemonSaveGame *SaveGame);

    /**
     * Event triggered on the first attempt to save the game.
     * This method can be implemented in Blueprints to define custom behavior that occurs
     * when the player tries to save the game for the first time.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Saving)
    void OnFirstSaveAttempt();

    /**
     * Prompt the player to save the game.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Saving)
    void PromptToSaveGame();

  public:
    /**
     * Attempt to save the game
     */
    UE5Coro::TCoroutine<bool> SaveGame();

    RETRO_MULTICAST_DELEGATE_MEMBER(FExitSaveScreen, OnExitSaveScreen)

    UE5Coro::TCoroutine<bool> UntilSaveComplete();

  protected:
    /**
     * Exits the save screen and broadcasts the result of the save operation.
     * This function closes the screen and triggers the OnExitSaveScreen event,
     * indicating whether the save operation was successful or not.
     *
     * @param bSuccess A boolean flag indicating the success (true) or failure (false) of the save operation.
     */
    UFUNCTION(BlueprintCallable, Category = Saving)
    void ExitSaveScreen(bool bSuccess);

  private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USaveGameCard> SaveGameCard;

    UPROPERTY()
    TObjectPtr<UEnhancedSaveGame> CurrentSaveGame;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, USaveScreen)
