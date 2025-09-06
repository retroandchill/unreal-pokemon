// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/NamedSlotSelectableWidget.h"
#include "Components/SelectableWidget.h"

#include "PokemonActionOptions.generated.h"

class UPokemonBattleScreen;
class IBattler;
class UBattleMenuOption;
class UBattleMenuHandler;
/**
 * The window for selecting a Pokémon's initial actions in the battle system.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonActionOptions : public UNamedSlotSelectableWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Battler)
    const TScriptInterface<IBattler> &GetCurrentBattler() const;

    /**
     * Set the current battler for this widget
     * @param Battler The current battler
     */
    void SetBattler(const TScriptInterface<IBattler> &Battler);

  private:
    /**
     * The current battler who is selecting
     */
    UPROPERTY(BlueprintGetter = GetCurrentBattler, Category = Battler)
    TScriptInterface<IBattler> CurrentBattler;
};
