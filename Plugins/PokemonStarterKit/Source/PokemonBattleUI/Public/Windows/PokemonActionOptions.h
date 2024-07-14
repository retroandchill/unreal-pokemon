// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"

#include "PokemonActionOptions.generated.h"

class UPokemonBattleScreen;
class IBattler;
class UBattleMenuOption;
class UBattleMenuHandler;
/**
 * The window for selecting a Pokémon's initial actions in the battle system.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonActionOptions : public USelectableWidget {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;

public:
    const TScriptInterface<IBattler> &GetCurrentBattler() const;

    /**
     * Set the current battler for this widget
     * @param Battler The current battler
     */
    void SetBattler(const TScriptInterface<IBattler> &Battler);

    /**
     * Execute the handler at the current index
     * @param Screen The battle context to invoke for the handler
     */
    void ExecuteCurrentHandler(UPokemonBattleScreen *Screen);

  private:
    /**
     * Take a menu handler and create an option out of it
     * @param MenuHandler The handler to use as the blueprint for making the option
     * @return The created option
     */
    void CreateMenuOption(const UBattleMenuHandler *MenuHandler);

    /**
     * The current battler who is selecting
     */
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    /**
     * The class used to spawn options into the menu
     */
    UPROPERTY(EditAnywhere, Category = Selection)
    TSubclassOf<UBattleMenuOption> OptionClass;

    /**
     * The handlers for the actions used to create the options in question
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Selection)
    TArray<TObjectPtr<UBattleMenuHandler>> MenuActions;
};
