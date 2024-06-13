// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonBattlePanel.h"
#include "PokemonBattlePanelPlayer.generated.h"

class UNumberImageWidget;
/**
 * The battle pannel for the player. Displays more information than the regular one.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonBattlePanelPlayer : public UPokemonBattlePanel {
    GENERATED_BODY()

public:
    void Refresh() override;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNumberImageWidget> CurrentHP;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UNumberImageWidget> MaxHP;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;

};
