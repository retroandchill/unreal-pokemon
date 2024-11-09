// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Common/PokemonStatDisplay.h"
#include "Components/Common/PokemonStatEntry.h"
#include "Components/Common/PokemonStatGraph.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Views/Map.h"

TSharedRef<SWidget> UPokemonStatDisplay::RebuildWidget() {
    // clang-format off
    StatGraph = SNew(SPokemonStatGraph)
        .StatNames(StatOrder)
        .GridLinesColor(FLinearColor::Gray)
        .NodeLinesColor(FLinearColor::White);
    // clang-format on
    return StatGraph.ToSharedRef();
}

void UPokemonStatDisplay::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    StatGraph.Reset();
}

void UPokemonStatDisplay::GetSlotNames(TArray<FName> &SlotNames) const {
    // clang-format off
    SlotNames = StatOrder |
                UE::Ranges::Map(&FMainStatHandle::RowID) |
                UE::Ranges::ToArray;
    // clang-format on
}

UWidget *UPokemonStatDisplay::GetContentForSlot(FName SlotName) const {
    // clang-format off
    return UE::Optionals::OfNullable(StatEntries.Find(SlotName)) |
           UE::Optionals::Map(&TObjectPtr<UWidget>::Get) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
}

void UPokemonStatDisplay::SetContentForSlot(FName SlotName, UWidget *Content) {
    StatEntries.Emplace(SlotName, Content);
}