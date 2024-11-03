// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/Delegates.h"
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
    void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

    /**
     * Set the save game being used by this window
     * @param SaveGame The save game being used by the window
     */
    UFUNCTION(BlueprintCallable, Category = Saving)
    void SetSaveGame(UEnhancedSaveGame *SaveGame);

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

    UE_MULTICAST_DELEGATE_MEMBER(FExitSaveScreen, OnExitSaveScreen)

  protected:
    UFUNCTION(BlueprintCallable, Category = Saving)
    void ExitSaveScreen(bool bSuccess);

  private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USaveGameCard> SaveGameCard;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> LastSavedText;

    UPROPERTY()
    TObjectPtr<UEnhancedSaveGame> CurrentSaveGame;

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

    FOnSaveComplete OnSaveCompleteDelegate;

    TOptional<TFuture<UEnhancedSaveGame *>> SaveGameCreationFuture;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, USaveScreen)
