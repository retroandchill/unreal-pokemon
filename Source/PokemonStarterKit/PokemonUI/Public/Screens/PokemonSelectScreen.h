// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PartyDisplayScreen.h"
#include "RetroLib/Functional/Delegates.h"
#include "Screens/PartyScreen.h"
#include "Screens/Screen.h"
#include "Trainers/Trainer.h"
#include "Utilities/CommandStack.h"

#include "PokemonSelectScreen.generated.h"

class UPokemonSelectionPaneBase;
class UPartyMenuHandlerSet;
class UPartyMenuHandler;
class UCommand;
class UCommandWindow;
class UHelpWindow;
class UPokemonSelectionPane;

/**
 * Delegate for when an item is selected
 */
DECLARE_DELEGATE_OneParam(FOnPokemonSelected, TOptional<FSelectedPokemonHandle>);

/**
 * Screen for when the player needs to select a Pokémon from the menu
 */
UCLASS(Blueprintable, Abstract, DisplayName = "Pokemon Select Screen (Deprecated)")
class POKEMONUI_API UPokemonSelectScreen : public UScreen, public IPartyScreen, public IPartyDisplayScreen
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UPokemonSelectScreen *AddPokemonSelectScreenToStack(const UObject *WorldContextObject);

  protected:
    void NativeConstruct() override;

  public:
    UFUNCTION(BlueprintCallable, Category = Switching)
    void BeginSwitch(int32 Index) final;

    UFUNCTION(BlueprintPure, Category = Owner)
    APlayerController *GetPlayerController() const final;

    UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>> PromptPokemonSelection() override;

    UFUNCTION(BlueprintCallable, Category = Display)
    void RefreshScene() override;

  protected:
    void RefreshSelf_Implementation() override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UPokemonSelectionPane *GetSelectionPane() const;

    UPokemonSelectionPaneBase *GetPokemonSelectionPane_Implementation() const override;

  public:
    UFUNCTION(BlueprintCallable, Category = Navigation)
    void RemoveFromStack() override;

    void CloseScreen() override;

  protected:
    /**
     * Callback for when a Pokémon in selected
     * @param Index The index that was selected
     */
    UFUNCTION(BlueprintCallable, Category = Selection)
    void OnPokemonSelected(int32 Index);

    UFUNCTION(BlueprintCallable, Category = Selection)
    void OnPokemonCancel();

    /**
     * Display the commands available for a selected Pokémon in the command window
     * @param Trainer The trainer that owns the selected Pokémon
     * @param Index The index of the selected Pokémon in the trainer's party
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Selection)
    void DisplayPokemonCommands(const TScriptInterface<ITrainer> &Trainer, int32 Index);

  private:
    /**
     * Delegate for when a Pokémon is selected
     */
    FOnPokemonSelected PokemonSelected;

    /**
     * The "Window" that the player selects a Pokémon from
     */
    UPROPERTY(BlueprintGetter = GetSelectionPane, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UPokemonSelectionPane> SelectionPane;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UPokemonSelectScreen)