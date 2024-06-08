// "Unreal Pokémon" created by Retro & Chill.

#include "Memory/CursorMemorySubsystem.h"

void UCursorMemorySubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    PocketNames = UItemHelper::GetPocketNames();
    BagPocketMemory.Empty();
    for (auto Pocket : PocketNames) {
        BagPocketMemory.Add(Pocket, 0);
    }
}

const TMap<FName, int32> &UCursorMemorySubsystem::GetBagPocketMemory() const {
    return BagPocketMemory;
}

void UCursorMemorySubsystem::UpdatePocketMemory(FName Pocket, int32 NewIndex) {
    BagPocketMemory.FindOrAdd(Pocket) = NewIndex;
}

const TArray<FName> &UCursorMemorySubsystem::GetBagPocketNames() const {
    return PocketNames;
}