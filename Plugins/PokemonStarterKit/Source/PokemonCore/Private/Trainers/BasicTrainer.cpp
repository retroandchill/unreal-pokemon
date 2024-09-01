// "Unreal Pokémon" created by Retro & Chill.

#include "Trainers/BasicTrainer.h"
#include "Algo/ForEach.h"
#include "DataManager.h"
#include "Lookup/InjectionUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"

TScriptInterface<ITrainer> UBasicTrainer::Initialize(FName NewTrainerType, FText NewTrainerName) {
    InternalId = FGuid::NewGuid();
    TrainerType = NewTrainerType;
    Name = NewTrainerName;
    ID = FMath::RandRange(0, 999999);
    SecretID = FMath::RandRange(0, 999999);
    return this;
}

TScriptInterface<ITrainer> UBasicTrainer::Initialize(const FTrainerDTO &DTO) {
    InternalId = DTO.InternalID;
    TrainerType = DTO.TrainerType;
    Name = DTO.Name;
    ID = DTO.ID;
    SecretID = DTO.SecretID;

    // clang-format off
    Party = DTO.Party |
            UE::Ranges::Map([this](const FPokemonDTO &Pokemon) {
                return UnrealInjector::NewInjectedDependency<IPokemon>(this, Pokemon);
            }) |
            UE::Ranges::ToArray;
    // clang-format on
    return this;
}

FTrainerDTO UBasicTrainer::ToDTO() const {
    return {.InternalID = InternalId,
            .TrainerType = TrainerType,
            .Name = Name,
            // clang-format off
            .Party = Party |
                     UE::Ranges::Map(&IPokemon::ToDTO) |
                     UE::Ranges::ToArray,
            // clang-format on
            .ID = ID,
            .SecretID = SecretID};
}

const FGuid &UBasicTrainer::GetInternalId() const {
    return InternalId;
}

const FTrainerType &UBasicTrainer::GetTrainerType() const {
    const auto &DataManager = FDataManager::GetInstance();
    auto &TrainerTypeTable = DataManager.GetDataTable<FTrainerType>();

    auto TrainerTypeData = TrainerTypeTable.GetData(TrainerType);
    check(TrainerTypeData != nullptr)
    return *TrainerTypeData;
}

FText UBasicTrainer::GetTrainerName() const {
    return Name;
}

FText UBasicTrainer::GetFullTrainerName() const {
    return FText::Format(FText::FromStringView(TEXT("{0} {1}")), {GetTrainerType().RealName, Name});
}

int32 UBasicTrainer::GetPayout() const {
    check(!Party.IsEmpty())
    return GetTrainerType().BaseMoney * Party.Last()->GetStatBlock()->GetLevel();
}

void UBasicTrainer::HealParty() {
    Algo::ForEach(Party, [](const TScriptInterface<IPokemon> &Pokemon) { Pokemon->FullyHeal(); });
}

const TArray<TScriptInterface<IPokemon>> &UBasicTrainer::GetParty() const {
    return Party;
}

TScriptInterface<IPokemon> UBasicTrainer::GetPokemon(int32 Index) const {
    if (!Party.IsValidIndex(Index)) {
        return nullptr;
    }

    return Party[Index];
}

int32 UBasicTrainer::GetAblePokemonCount() const {
    int32 Count = 0;
    for (auto &Pokemon : Party) {
        if (!Pokemon->IsFainted()) {
            Count++;
        }
    }

    return Count;
}

void UBasicTrainer::AddPokemonToParty(const TScriptInterface<IPokemon> &Pokemon) {
    Party.Add(Pokemon);
    Pokemon->SetCurrentHandler(this);
}

void UBasicTrainer::SwapPositionsInParty(int32 Index1, int32 Index2) {
    check(Index1 >= 0 && Index1 < Party.Num() && Index2 >= 0 && Index2 < Party.Num())
    Swap(Party[Index1], Party[Index2]);
}

void UBasicTrainer::ClearParty() {
    Party.Empty();
}

int32 UBasicTrainer::GetIdNumber() const {
    return ID;
}

int32 UBasicTrainer::GetSecretId() const {
    return SecretID;
}