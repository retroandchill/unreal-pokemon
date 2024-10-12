// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/DefaultStorageSystem.h"
#include "Lookup/InjectionUtilities.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/Construct.h"
#include "Ranges/Optional/FlatMapTuple.h"
#include "Ranges/Views/Enumerate.h"
#include "Ranges/Views/FilterTuple.h"
#include "Ranges/Views/Ints.h"
#include "Ranges/Views/Join.h"
#include "Ranges/Views/Map.h"
#include "Settings/PokemonStorageSystemSettings.h"
#include "Storage/StorageBox.h"

static FText GetDefaultBoxName(int32 Index);
static bool CheckOpenBox(int32 Index, const TScriptInterface<IStorageBox> &Box);
static TOptional<FDepositResult> TryDepositToBox(int32 Index, const TScriptInterface<IStorageBox> &Box,
    const TScriptInterface<IPokemon> &Pokemon);

TScriptInterface<IStorageSystem> UDefaultStorageSystem::Initialize(int32 BoxCount, int32 BoxCapacity,
                                                                   int32 StartingIndex) {
    // clang-format off
    Boxes = UE::Ranges::Ints(0, BoxCount) |
            UE::Ranges::Map(&GetDefaultBoxName) |
            UE::Ranges::Map(this, &UDefaultStorageSystem::CreateStorageBox, BoxCapacity) |
            UE::Ranges::ToArray;
    // clang-format on
    CurrentBoxIndex = StartingIndex;
    check(Boxes.IsValidIndex(CurrentBoxIndex))
    return this;
}

TScriptInterface<IStorageSystem> UDefaultStorageSystem::Initialize(const FStorageSystemDTO &DTO) {
    // clang-format off
    Boxes = DTO.Boxes |
            UE::Ranges::Map(&FStorageBoxDTO::CreateBox, this) |
            UE::Ranges::ToArray;
    // clang-format on
    CurrentBoxIndex = DTO.CurrentBoxIndex;
    check(Boxes.IsValidIndex(CurrentBoxIndex))
    return this;
}

FStorageSystemDTO UDefaultStorageSystem::ToDTO() const {
    return {
        // clang-format off
        .Boxes = Boxes |
                 UE::Ranges::Map(&IStorageBox::ToDTO) |
                 UE::Ranges::ToArray,
        // clang-format on
        .CurrentBoxIndex = CurrentBoxIndex};
}

int32 UDefaultStorageSystem::GetBoxCount() const {
    return Boxes.Num();
}

const TScriptInterface<IStorageBox> &UDefaultStorageSystem::GetBox(int32 Index) const {
    check(Boxes.IsValidIndex(Index))
    return Boxes[Index];
}

const TScriptInterface<IStorageBox> & UDefaultStorageSystem::GetCurrentBox() const {
    return GetBox(CurrentBoxIndex);
}

int32 UDefaultStorageSystem::GetCurrentBoxIndex() const {
    return CurrentBoxIndex;
}

void UDefaultStorageSystem::SetCurrentBoxIndex(int32 NewIndex) {
    check(Boxes.IsValidIndex(NewIndex))
    CurrentBoxIndex = NewIndex;
}

TOptional<FDepositResult> UDefaultStorageSystem::TryDeposit(const TScriptInterface<IPokemon> &Pokemon) {
    check(Boxes.IsValidIndex(CurrentBoxIndex))
    std::array Indexes = {UE::Ranges::Ints(CurrentBoxIndex, Boxes.Num()), UE::Ranges::Ints(0, CurrentBoxIndex)};
    // clang-format off
    return Indexes |
                   UE::Ranges::Join |
                   UE::Ranges::ReverseEnumerate<int32>(Boxes) |
                   UE::Ranges::FilterTuple(&CheckOpenBox) |
                   UE::Ranges::FindFirst |
                   UE::Optionals::FlatMapTuple(&TryDepositToBox, Pokemon);
    // clang-format on
}

TScriptInterface<IStorageBox> UDefaultStorageSystem::CreateStorageBox(FText &&BoxName, int32 BoxCapacity) {
    return UnrealInjector::NewInjectedDependency<IStorageBox>(this, MoveTemp(BoxName), BoxCapacity);
}

static FText GetDefaultBoxName(int32 Index) {
    auto Settings = GetDefault<UPokemonStorageSystemSettings>();
    return FText::FormatNamed(Settings->BoxNameFormat, TEXT("Index"), Index + 1);
}

static bool CheckOpenBox(int32, const TScriptInterface<IStorageBox> &Box) {
    return !Box->IsBoxFull();
}

static TOptional<FDepositResult> TryDepositToBox(int32 Index, const TScriptInterface<IStorageBox> &Box,
    const TScriptInterface<IPokemon> &Pokemon) {
    return Box->DepositToBox(Pokemon) |
        UE::Optionals::Construct<FDepositResult>(Index);
}