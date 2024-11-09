// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Common/PokemonStatDisplay.h"
#include "Components/Common/PokemonStatGraph.h"

TSharedRef<SWidget> UPokemonStatDisplay::RebuildWidget() {
    StatGraph = SNew(SPokemonStatGraph)
        .StatNames_Static(&UStatHelper::GetMainStatNames)
        .GridLinesColor(FLinearColor::Gray)
        .NodeLinesColor(FLinearColor::White);
    return StatGraph.ToSharedRef();
}

void UPokemonStatDisplay::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    StatGraph.Reset();
}