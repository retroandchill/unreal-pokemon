// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PartyScreen.h"
#include "Screens/Screen.h"
#include "Trainers/Trainer.h"
#include "Utilities/CommandStack.h"

#include "PokemonSelectScreen.generated.h"

class UPartyMenuHandlerSet;
class UPartyMenuHandler;
class UCommand;
class UCommandWindow;
class UHelpWindow;
class UPokemonSelectionPane;

/**
 * Screen for when the player needs to select a Pokémon from the menu
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UPokemonSelectScreen : public UScreen, public IPartyScreen {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
    UFUNCTION(BlueprintCallable, Category = Switching)
    void BeginSwitch(int32 Index) final;

    void ShowCommands(const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers) override;

    UFUNCTION(BlueprintCallable, Category = Display)
    void ClearCommandStack() override;

    UFUNCTION(BlueprintCallable, Category = Display)
    void SetCommandHelpText(FText Text) final;

    UFUNCTION(BlueprintPure, Category = Owner)
    APlayerController *GetPlayerController() const final;

    FOnPokemonSelected &GetOnPokemonSelect() override;

    UFUNCTION(BlueprintCallable, Category = Display)
    void RefreshScene() override;

  protected:
    void RefreshSelf_Implementation() override;

  public:
    /**
     * Set the help text for the regular help window
     * @param Text The text to set to the window
     */
    UFUNCTION(BlueprintCallable, Category = "Display")
    void SetHelpText(FText Text);

    UFUNCTION(BlueprintCallable, Category = Navigation)
    void RemoveFromStack() override;

  private:
    /**
     * Callback for when a Pokémon in selected
     * @param Index The index that was selected
     */
    UFUNCTION()
    void OnPokemonSelected(int32 Index);

    /**
     * Display the commands available for a selected Pokémon in the command window
     * @param Trainer The trainer that owns the selected Pokémon
     * @param Index The index of the selected Pokémon in the trainer's party
     */
    void DisplayPokemonCommands(const TScriptInterface<ITrainer> &Trainer, int32 Index);

    /**
     * Process the selected command
     * @param CurrentIndex The index of the command
     * @param SelectedCommand The command in question
     */
    UFUNCTION()
    void ProcessCommand(int32 CurrentIndex, UCommand *SelectedCommand);

    /**
     * Callback for when the player cancels from the command window
     */
    UFUNCTION()
    void OnCommandWindowCancel();

    /**
     * Display the area the command window is contained in
     * @param bIsVisible Should the window be shown
     */
    void ToggleCommandWindowVisibility(bool bIsVisible);

    /**
     * Delegate for when a Pokémon is selected
     */
    FOnPokemonSelected PokemonSelected;

    /**
     * The "Window" that the player selects a Pokémon from
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPokemonSelectionPane> SelectionPane;

    /**
     * The help window used for when the commands are shown.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHelpWindow> HelpWindow;

    /**
     * The window that contains the command to call on a Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommandWindow> CommandWindow;

    /**
     * The help window used for when the commands are shown.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHelpWindow> CommandHelpWindow;

    /**
     * The handlers for the command window when a Pokémon in selected
     */
    UPROPERTY(EditAnywhere, Category = Commands)
    TOptional<FText> CancelText;

    /**
     * The handlers for the command window when a Pokémon in selected
     */
    UPROPERTY(EditAnywhere, Category = Commands)
    TObjectPtr<UPartyMenuHandlerSet> PokemonHandlers;

    /**
     * The stack frame used to track the commands being held
     */
    UPROPERTY()
    TArray<FCommandStackFrame> CommandStack;
};
