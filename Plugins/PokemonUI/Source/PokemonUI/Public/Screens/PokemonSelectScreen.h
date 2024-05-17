// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PartyScreen.h"
#include "Screens/Screen.h"
#include "Trainers/Trainer.h"

#include "PokemonSelectScreen.generated.h"

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
    void BeginSwitch(int32 Index) final;
    void SetHelpText(const FText &Text) final;
    ARPGPlayerController &GetPlayerController() final;

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
    void DisplayPokemonCommands(ITrainer &Trainer, int32 Index);

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
     * The "Window" that the player selects a Pokémon from
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPokemonSelectionPane> SelectionPane;

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
    FText CancelText;

    /**
     * The handlers for the command window when a Pokémon in selected
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Commands)
    TArray<TObjectPtr<UPartyMenuHandler>> PokemonHandlers;
};
