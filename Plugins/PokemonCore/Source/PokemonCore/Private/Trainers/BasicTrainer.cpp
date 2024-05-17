// "Unreal Pokémon" created by Retro & Chill.

#include "Trainers/BasicTrainer.h"
#include "DataManager.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"

TScriptInterface<ITrainer> UBasicTrainer::Initialize(FName NewTrainerType, FText NewTrainerName) {
    TrainerType = NewTrainerType;
    Name = NewTrainerName;
    ID = FMath::RandRange(0, 999999);
    SecretID = FMath::RandRange(0, 999999);
    return this;
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

int32 UBasicTrainer::GetPayout() const {
    if (Party.IsEmpty()) {
        return 0;
    }
    return GetTrainerType().BaseMoney * Party.Last()->GetStatBlock()->GetLevel();
}

const TArray<TScriptInterface<IPokemon>> &UBasicTrainer::GetParty() const {
    return Party;
}

void UBasicTrainer::AddPokemonToParty(const TScriptInterface<IPokemon> &Pokemon) {
    Party.Add(Pokemon);
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
