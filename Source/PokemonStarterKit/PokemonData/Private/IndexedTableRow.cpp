// "Unreal Pokémon" created by Retro & Chill.

#include "IndexedTableRow.h"

void FIndexedTableRow::OnDataTableChanged(const UDataTable *InDataTable, const FName InRowName)
{
    ID = InRowName;
    RowID = InDataTable->GetRowNames().IndexOfByKey(ID) + 1;
}