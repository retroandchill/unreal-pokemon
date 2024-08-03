// "Unreal Pokémon" created by Retro & Chill.


#include "Management/RandomEncounterSubsystem.h"
#include "Algo/ForEach.h"
#include "EncounterData/MapEncounterData.h"

void URandomEncounterSubsystem::SetEncounterData(AMapEncounterData *Data) {
    EncounterData = Data;
}

bool URandomEncounterSubsystem::RequestEncounterForType(FName EncounterType, FRetrievedEncounter &Encounter, int32 ChanceRolls) const {
    if (!IsValid(EncounterData)) {
        return false;
    }

    auto &Encounters = EncounterData->GetEncounters();
    auto Data = Encounters.Find(EncounterType);
    if (Data == nullptr) {
        return false;
    }

    auto EncounterList = Data->Encounters;
    EncounterList.Sort([](const FEncounterEntry& A, const FEncounterEntry& B) { return A.Chance > B.Chance; });
    int32 ChanceTotal = 0;
    Algo::ForEach(EncounterList, [&ChanceTotal](const FEncounterEntry& Entry) { ChanceTotal += Entry.Chance;});

    int32 RandomNumber = 0;
    for (int32 i = 0; i < ChanceRolls; i++) {
        if (int32 Value = FMath::Rand() % ChanceTotal; Value > RandomNumber) {
            RandomNumber = Value;
        }
    }

    FEncounterEntry* EncounterEntry = nullptr;
    for (auto &Entry : EncounterList) {
        RandomNumber -= Entry.Chance;
        if (RandomNumber >= 0) {
            continue;
        }

        EncounterEntry = &Entry;
        break;
    }

    check(EncounterEntry != nullptr)
    Encounter.Species = EncounterEntry->Species;
    Encounter.Level = FMath::RandRange(EncounterEntry->LevelRange.GetLowerBoundValue(), EncounterEntry->LevelRange.GetUpperBoundValue());
    return true;
}