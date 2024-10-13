// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/Delegates.h"
#include "Screens/Screen.h"

#include "MoveForgetScreen.generated.h"

struct FMoveHandle;
class IMove;
class UMoveSelectWindow;
class UMoveInfoWindow;
class IPokemon;

/**
 * Delegate called when a move is forgotten by a move completion
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveForgetComplete, bool);

/**
 * The screen used for having a Pokémon request to forget a move
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UMoveForgetScreen : public UScreen {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject, AutoCreateRefTerm = Move))
    static UMoveForgetScreen* AddMoveForgetScreenToStack(const UObject* WorldContextObject);

  protected:
    void NativeConstruct() override;

  public:
    /**
     * Initialize the screen with only the Pokémon's existing moves
     * @param Pokemon The Pokémon that will forget a move
     */
    void InitializeScene(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Initialize the screen attempting to learn a new move
     * @param Pokemon The Pokémon that is attempting to learn a new move
     * @param Move The ID of the move that the Pokémon is attempting to learn
     */
    void InitializeScene(const TScriptInterface<IPokemon> &Pokemon, FName Move);

    /**
     * Called when the move deletion is completed
     */
    UE_MULTICAST_DELEGATE_MEMBER(FOnMoveForgetComplete, OnMoveForgetComplete)

  protected:
    /**
     * Complete the move forget prompt and exit the scene
     * @param bMoveForgotten Was the move forgotten
     */
    UFUNCTION(BlueprintCallable, Category = "Scene|Exit")
    void MoveForgetComplete(bool bMoveForgotten);

    /**
     * Get the window used to actually select the moves
     * @return The window used to actually select the moves
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UMoveSelectWindow *GetMoveSelectWindow() const;

    /**
     * Get the window that actively displays the move info
     * @return The window that actively displays the move info
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UMoveInfoWindow *GetMoveInfoWindow() const;

    /**
     * Refresh the layout of the screen
     * @param Pokemon
     * @param bIsLearningMove Is the Pokémon trying to learn a move
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void RefreshLayout(const TScriptInterface<IPokemon> &Pokemon, bool bIsLearningMove);

    /**
     * Process confirming asking the player to confirm forgetting a move
     * @param Pokemon The Pokémon trying to forget a move
     * @param MoveIndex The index of the move the player is trying to forget
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Pokémon|Moves")
    void PromptMoveForget(const TScriptInterface<IPokemon> &Pokemon, int32 MoveIndex);

    /**
     * Prompt giving up attempting to learn a move
     * @param Pokemon The Pokémon trying to forget a move
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Pokémon|Moves")
    void PromptGiveUpForget(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Prompt the player to forget the currently selected move and learn a new move in its place
     * @param Pokemon The Pokémon trying to learn a move
     * @param MoveIndex The index of the move to forget
     * @param NewMove The ID of the new move to learn
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Pokémon|Moves")
    void PromptMoveLearn(const TScriptInterface<IPokemon> &Pokemon, int32 MoveIndex, FName NewMove);

    /**
     * Prompt the player to give up trying to learn a new move
     * @param Pokemon The Pokémon trying to learn a move
     * @param Move The ID of the new move to learn
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Pokémon|Moves")
    void PromptGiveUpLearn(const TScriptInterface<IPokemon> &Pokemon, FName Move);

  private:
    /**
     * Called when a move is selected
     * @param Index The index of the move that is was selected
     */
    UFUNCTION()
    void OnMoveSelected(int32 Index);

    /**
     * Called when the cancel button is selected
     */
    UFUNCTION()
    void OnCanceled();

    /**
     * The Pokémon that is trying to forget a move
     */
    UPROPERTY()
    TScriptInterface<IPokemon> OwnedPokemon;

    /**
     * The move that is trying to be taught
     */
    TOptional<FName> MoveToLearn;

    /**
     * The window used to actually select the moves
     */
    UPROPERTY(BlueprintGetter = GetMoveSelectWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UMoveSelectWindow> MoveSelectWindow;

    /**
     * The window that actively displays the move info
     */
    UPROPERTY(BlueprintGetter = GetMoveInfoWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UMoveInfoWindow> MoveInfoWindow;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UMoveForgetScreen)