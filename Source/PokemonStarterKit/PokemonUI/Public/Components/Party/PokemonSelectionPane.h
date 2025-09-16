// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Components/Party/PokemonPanel.h"
#include "Components/SelectableWidget.h"

#include "PokemonSelectionPane.generated.h"

/**
 * Pane used to select Pokémon from in a grid based format
 */
UCLASS(Abstract, Blueprintable, DisplayName = "Pokemon Selection Pane (Deprecated)")
class POKEMONUI_API UPokemonSelectionPane : public UPokemonSelectionPaneBase
{
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;
    TOptional<int32> GetNumPanelsToAdd() const override;
};
