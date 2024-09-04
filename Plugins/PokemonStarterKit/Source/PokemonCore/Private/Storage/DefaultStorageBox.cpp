// "Unreal Pokémon" created by Retro & Chill.


#include "Storage/DefaultStorageBox.h"
#include "Ranges/Algorithm/AllOf.h"
#include "Ranges/Algorithm/AnyOf.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Ranges/Views/Map.h"
#include "Storage/StorageBoxDTO.h"
#include "Ranges/Views/ContainerView.h"
#include "Utilities/TrainerHelpers.h"

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(FText &&InitialName, int32 Capacity) {
    DisplayName = MoveTemp(InitialName);
    StoredPokemon.SetNum(Capacity);
    return this;
}

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(const FStorageBoxDTO &DTO) {
    DisplayName = DTO.DisplayName;

    auto Player = UTrainerHelpers::GetPlayerCharacter(this);
    check(Player != nullptr)
    // clang-format off
    StoredPokemon = DTO.StoredPokemon |
                    UE::Ranges::Map(&FStorageSlot::Pokemon) |
                    UE::Ranges::Map([&Player](const TOptional<FPokemonDTO> &Pokemon) {
                        return Pokemon |
                               UE::Optionals::Map(&FPokemonDTO::CreatePokemon, Player.GetObject()) |
                               UE::Optionals::OrElseGet([] {
                                   return TScriptInterface<IPokemon>();
                               });
                    }) |
                    UE::Ranges::ToArray;
    // clang-format on

    return this;
}

const FText &UDefaultStorageBox::GetDisplayName() const {
    return DisplayName;
}

void UDefaultStorageBox::SetDisplayName(const FText &NewName) {
    DisplayName = NewName;
}

TOptional<int32> UDefaultStorageBox::DepositToBox(const TScriptInterface<IPokemon> &Pokemon) {
    int32 TargetIndex = StoredPokemon.IndexOfByKey(nullptr);
    if (TargetIndex == INDEX_NONE) {
        return TOptional<int32>();
    }

    StoredPokemon[TargetIndex] = Pokemon;
    return TargetIndex;
}

bool UDefaultStorageBox::IsBoxFull() const {
    // clang-format off
    return StoredPokemon |
           UE::Ranges::Map(&TScriptInterface<IPokemon>::GetObject) |
           UE::Ranges::AllOf(&IsValid);
    // clang-format on
}

TOptional<IPokemon &> UDefaultStorageBox::WithdrawFromBox(int32 BoxIndex) {
    check(StoredPokemon.IsValidIndex(BoxIndex))
    TOptional<IPokemon &> ExistingPokemon(StoredPokemon[BoxIndex].GetInterface());
    StoredPokemon[BoxIndex] = nullptr;
    return ExistingPokemon;
}

TOptional<IPokemon &> UDefaultStorageBox::SwapWithPokemon(int32 BoxIndex, const TScriptInterface<IPokemon> &Pokemon) {
    check(Pokemon != nullptr)
    check(StoredPokemon.IsValidIndex(BoxIndex))
    TOptional<IPokemon &> ExistingPokemon(StoredPokemon[BoxIndex].GetInterface());
    StoredPokemon[BoxIndex] = Pokemon;
    return ExistingPokemon;
}