// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonInfoWidget.h"
#include "PokemonStatDisplay.generated.h"

class SPokemonStatGraph;
/**
 * 
 */
UCLASS()
class POKEMONUI_API UPokemonStatDisplay : public UWidget {
    GENERATED_BODY()

public:
    TSharedRef<SWidget> RebuildWidget() override;

    void ReleaseSlateResources(bool bReleaseChildren) override;

private:
    TSharedPtr<SPokemonStatGraph> StatGraph;

};
