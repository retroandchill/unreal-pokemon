// "Unreal Pokémon" created by Retro & Chill.


#include "Storage/DefaultStorageBox.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Ranges/Views/Map.h"
#include "Storage/StorageBoxDTO.h"
#include "Ranges/Views/ContainerView.h"
#include "Utilities/TrainerHelpers.h"

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(FText &&InitialName, int32 Capacity) : DisplayName(MoveTemp(InitialName)) {
    StoredPokemon.SetNum(Capacity);
    return this;
}

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(const FStorageBoxDTO &DTO) {
    DisplayName = DTO.DisplayName;

    auto Player = UTrainerHelpers::GetPlayerCharacter(this);
    check(Player != nullptr)
    // clang-format off
    StoredPokemon = DTO.StoredPokemon |
        UE::Ranges::Map([&Player](const TOptional<FPokemonDTO>& Pokemon) {
            return Pokemon |
                UE::Optionals::Map(&FPokemonDTO::CreatePokemon, Player.GetObject()) |
                UE::Optionals::OrElseGet([] { return TScriptInterface<IPokemon>(); });    
        }) |
        UE::Ranges::ToArray;
    // clang-format on

    return this;
}

const FText & UDefaultStorageBox::GetDisplayName() const {
    return DisplayName;
}

void UDefaultStorageBox::SetDisplayName(const FText &NewName) {
    DisplayName = NewName;
}