// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/DefaultStorageSystem.h"
#include "Lookup/InjectionUtilities.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Compatibility/Array.h"
#include "RetroLib/Ranges/Views/Enumerate.h"
#include "RetroLib/Utils/Construct.h"
#include "Settings/PokemonStorageSystemSettings.h"
#include "Storage/StorageBox.h"

static FText GetDefaultBoxName(int32 Index);
static bool CheckOpenBox(int32 Index, const TScriptInterface<IStorageBox> &Box);
static TOptional<FDepositResult> TryDepositToBox(int32 Index, const TScriptInterface<IStorageBox> &Box,
                                                 const TScriptInterface<IPokemon> &Pokemon);

TScriptInterface<IStorageSystem> UDefaultStorageSystem::Initialize(int32 BoxCount, int32 BoxCapacity,
                                                                   int32 StartingIndex) {
    // clang-format off
    Boxes = Retro::Ranges::Views::Iota(0, BoxCount) |
            Retro::Ranges::Views::Transform<&GetDefaultBoxName>() |
            Retro::Ranges::Views::Transform(this, &UDefaultStorageSystem::CreateStorageBox, BoxCapacity) |
            Retro::Ranges::To<TArray>();
    // clang-format on
    CurrentBoxIndex = StartingIndex;
    check(Boxes.IsValidIndex(CurrentBoxIndex))
    return this;
}

TScriptInterface<IStorageSystem> UDefaultStorageSystem::Initialize(const FStorageSystemDTO &DTO) {
    // clang-format off
    Boxes = DTO.Boxes |
            Retro::Ranges::Views::Transform<&FStorageBoxDTO::CreateBox>(this) |
            Retro::Ranges::To<TArray>();
    // clang-format on
    CurrentBoxIndex = DTO.CurrentBoxIndex;
    check(Boxes.IsValidIndex(CurrentBoxIndex))
    return this;
}

FStorageSystemDTO UDefaultStorageSystem::ToDTO() const {
    return {// clang-format off
        .Boxes = Boxes |
                 Retro::Ranges::Views::Transform<&IStorageBox::ToDTO>() |
                 Retro::Ranges::To<TArray>(),
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

const TScriptInterface<IStorageBox> &UDefaultStorageSystem::GetCurrentBox() const {
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
    std::array Indexes = {Retro::Ranges::Views::Iota(CurrentBoxIndex, Boxes.Num()),
                          Retro::Ranges::Views::Iota(0, CurrentBoxIndex)};
    // clang-format off
    auto View = Indexes |
           Retro::Ranges::Views::Join |
           Retro::Ranges::Views::ReverseEnumerate(Retro::Ranges::Views::All(Boxes));
    for (auto [Index, Value] : View) {
        if (CheckOpenBox(Index, Value)) {
            return TryDepositToBox(Index, Value, Pokemon);
        }
    }

    return TOptional<FDepositResult>();
    // clang-format on
}

TScriptInterface<IStorageBox> UDefaultStorageSystem::CreateStorageBox(FText &&BoxName, int32 BoxCapacity) {
    return UnrealInjector::NewInjectedDependency<IStorageBox>(this, std::move(BoxName), BoxCapacity);
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
    // clang-format off
    return Box->DepositToBox(Pokemon) |
           Retro::Optionals::Transform(Retro::Construct<FDepositResult>, Index);
    // clang-format on
}