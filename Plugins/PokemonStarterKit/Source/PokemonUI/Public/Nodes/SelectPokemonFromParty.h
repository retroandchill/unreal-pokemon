// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#include "Screens/Screen.h"

#include "SelectPokemonFromParty.generated.h"

class UPokemonSelectScreen;
class ITrainer;
class IPartyScreen;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPokemonSelectedDynamic, const TScriptInterface<IPartyScreen> &, Screen,
                                               const TScriptInterface<ITrainer> &, Trainer, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPokemonCancel);

/**
 * Open the menu to select a Pokémon from the party
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API USelectPokemonFromParty : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Open the menu to select a Pokémon from the party
     * @param WorldContextObject The object used to obtain the state of the world
     * @param ScreenClass The class used to display the selection screen
     * @param HelpText The text used to serve as the prompt to the player
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Selection")
    static USelectPokemonFromParty *SelectPokemonFromParty(const UObject *WorldContextObject, FText HelpText);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine = {}) override;

  private:
    /**
     * Called when the player selects a Pokémon
     */
    UPROPERTY(BlueprintAssignable)
    FPokemonSelectedDynamic OnSelected;

    /**
     * Called when the player cancels
     */
    UPROPERTY(BlueprintAssignable)
    FOnPokemonCancel OnCanceled;

    /**
     * The text used to serve as the prompt to the player
     */
    UPROPERTY()
    FText HelpText;
};