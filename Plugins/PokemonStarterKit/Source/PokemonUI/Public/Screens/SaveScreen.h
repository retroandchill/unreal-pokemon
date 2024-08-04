// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "SaveScreen.generated.h"

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

  protected:
    void NativeOnActivated() override;

    /**
     * Set the save game being used by this window
     * @param SaveGame The save game being used by the window
     */
    UFUNCTION(BlueprintCallable, Category = Saving)
    void SetSaveGame(UPokemonSaveGame *SaveGame);

    /**
     * Prompt the player to save the game.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Saving)
    void PromptToSaveGame();

  public:
    /**
     * Attempt to save the game
     */
    void SaveGame(FOnSaveComplete &&OnComplete);

    FDelegateHandle BindToExitSaveScreen(FExitSaveScreen::FDelegate &&Callback);

  protected:
    /**
     * Add any additional custom properties to the save file that might be needed.
     * @param SaveGame Additional properties to add to the save file
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Saving)
    void AddCustomSaveProperties(UPokemonSaveGame *SaveGame);

    UFUNCTION(BlueprintCallable, Category = Saving)
    void ExitSaveScreen(bool bSuccess);

  private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USaveGameCard> SaveGameCard;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> LastSavedText;

    UPROPERTY()
    TObjectPtr<UPokemonSaveGame> CurrentSaveGame;

    /**
     * The format used for the last saved blurb.
     */
    UPROPERTY(EditAnywhere, Category = Content)
    FText LastSavedFormat = NSLOCTEXT("PokemonUI", "SaveScreen_LastSavedFormat", "Last saved on {Date} at {Time}");

    /**
     * The format used for formatting the date
     */
    UPROPERTY(EditAnywhere, Category = "Memo|Formatting")
    FString DateFormat = TEXT("%m/%d/%Y");

    FExitSaveScreen OnExitSaveScreen;
};
