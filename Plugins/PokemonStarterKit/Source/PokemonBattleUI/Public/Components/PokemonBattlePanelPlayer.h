// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonBattlePanel.h"

#include "PokemonBattlePanelPlayer.generated.h"

class UNumberImageWidget;
/**
 * The battle panel for the player. Displays more information than the regular one.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonBattlePanelPlayer : public UPokemonBattlePanel {
    GENERATED_BODY()

  public:
    void Refresh() override;

  protected:
    void UpdateHPPercent(float NewPercent) override;
    void HPPercentUpdateComplete() const override;

  private:
    /**
     * The widget for displaying the current HP of the battler
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNumberImageWidget> CurrentHP;

    /**
     * The widget for displaying the max HP of the battler
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNumberImageWidget> MaxHP;

    /**
     * The widget for display the Exp percentage of the battler
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;
};
