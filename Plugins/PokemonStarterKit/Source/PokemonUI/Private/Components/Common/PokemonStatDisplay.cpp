// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonStatDisplay.h"
#include "Components/Common/PokemonStatGraph.h"

TSharedRef<SWidget> UPokemonStatDisplay::RebuildWidget() {
    // clang-format off
    StatGraph = SNew(SPokemonStatGraph)
        .StatNames(StatOrder)
        .GridLinesColor(InnerLinesColor)
        .NodeLinesColor(OutlineColor)
        .MinimumDesiredSize(MinimumDesiredSize);
    // clang-format on
    return StatGraph.ToSharedRef();
}

void UPokemonStatDisplay::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    StatGraph.Reset();
}

void UPokemonStatDisplay::SynchronizeProperties() {
    Super::SynchronizeProperties();
    if (StatGraph == nullptr) {
        return;
    }

    StatGraph->SetPokemon(Pokemon);
    StatGraph->SetGridLinesColor(InnerLinesColor);
    StatGraph->SetNodeLinesColor(OutlineColor);
    StatGraph->SetMinimumDesiredSize(MinimumDesiredSize);
}

void UPokemonStatDisplay::SetInnerLinesColor(const FLinearColor &InInnerLinesColor) {
    InnerLinesColor = InInnerLinesColor;
    SynchronizeProperties();
}

void UPokemonStatDisplay::SetOutlineColor(const FLinearColor &InOutlineColor) {
    OutlineColor = InOutlineColor;
    SynchronizeProperties();
}

void UPokemonStatDisplay::SetMinimumDesiredSize(const FVector2D &InMinimumDesiredSize) {
    MinimumDesiredSize = InMinimumDesiredSize;
    SynchronizeProperties();
}

void UPokemonStatDisplay::SetPokemon(const TScriptInterface<IPokemon> &NewPokemon) {
    Pokemon = NewPokemon;
    SynchronizeProperties();
}