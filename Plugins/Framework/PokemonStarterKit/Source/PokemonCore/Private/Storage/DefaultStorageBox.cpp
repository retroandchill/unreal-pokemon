// "Unreal Pokémon" created by Retro & Chill.

#include "Storage/DefaultStorageBox.h"
#include "Pokemon/Pokemon.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Utils/Construct.h"
#include "Storage/StorageBoxDTO.h"
#include "Utilities/TrainerHelpers.h"

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(FText &&InitialName, int32 Capacity) {
    DisplayName = std::move(InitialName);
    StoredPokemon.SetNum(Capacity);
    return this;
}

TScriptInterface<IStorageBox> UDefaultStorageBox::Initialize(const FStorageBoxDTO &DTO) {
    DisplayName = DTO.DisplayName;

    auto Player = UTrainerHelpers::GetPlayerCharacter(this);
    check(Player != nullptr)
    // clang-format off
    StoredPokemon = DTO.StoredPokemon |
                    Retro::Ranges::Views::Transform(&FStorageSlot::Pokemon) |
                    Retro::Ranges::Views::Transform([&Player](const TOptional<FPokemonDTO> &Pokemon) {
                        return Pokemon |
                               Retro::Optionals::Transform(Retro::BindBack<&FPokemonDTO::CreatePokemon>(Player.GetObject())) |
                               Retro::Optionals::OrElseGet([] {
                                   return TScriptInterface<IPokemon>();
                               });
                    }) |
                    Retro::Ranges::To<TArray>();
    // clang-format on

    return this;
}

FStorageBoxDTO UDefaultStorageBox::ToDTO() const {
    return {
        .DisplayName = DisplayName,
        // clang-format off
        .StoredPokemon = StoredPokemon |
                         Retro::Ranges::Views::Transform(&TScriptInterface<IPokemon>::GetInterface) |
                         Retro::Ranges::Views::Transform([](IPokemon *Pokemon) {
                             return Retro::Optionals::OfNullable(Pokemon) |
                                    Retro::Optionals::Transform(&IPokemon::ToDTO);
                         }) |
                         Retro::Ranges::Views::Transform(Retro::Construct<FStorageSlot>) |
                         Retro::Ranges::To<TArray>()
        // clang-format on
    };
}

const FText &UDefaultStorageBox::GetDisplayName() const {
    return DisplayName;
}

void UDefaultStorageBox::SetDisplayName(const FText &NewName) {
    DisplayName = NewName;
}

int32 UDefaultStorageBox::GetCapacity() const {
    return StoredPokemon.Num();
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
           Retro::Ranges::Views::Transform(&TScriptInterface<IPokemon>::GetObject) |
           Retro::Ranges::AllOf(&IsValid);
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

TOptional<IPokemon &> UDefaultStorageBox::GetStoredPokemon(int32 Index) const {
    check(StoredPokemon.IsValidIndex(Index))
    return Retro::Optionals::OfNullable(StoredPokemon[Index]);
}

const TArray<TScriptInterface<IPokemon>> &UDefaultStorageBox::GetStoredPokemon() const {
    return StoredPokemon;
}