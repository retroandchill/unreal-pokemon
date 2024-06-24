// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Type.h"
#include "DataManager.h"

void FType::OnDataTableChanged(const UDataTable *InDataTable, const FName InRowName) {
    FIndexedTableRow::OnDataTableChanged(InDataTable, InRowName);
    UseMoveTag = FGameplayTag::RequestGameplayTag(*FString::Format(TEXT("Battle.Moves.Types.{0}"), { ID.ToString() }), false);
    BattlerTag = FGameplayTag::RequestGameplayTag(*FString::Format(TEXT("Battle.Types.{0}"), { ID.ToString() }), false);
}

TArray<FName> UTypeHelper::GetTypeNames() {
    return FDataManager::GetInstance().GetDataTable<FType>().GetTableRowNames();
}
