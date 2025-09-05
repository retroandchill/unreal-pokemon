#include "GameDataRepository.h"

int32 UGameDataRepository::GetNumEntries() const
{
    return GameDataEntries->Num(); 
}

uint8 *UGameDataRepository::GetEntryAtIndex(const int32 Index) const
{
    return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;  
}

void UGameDataRepository::PostLoad() {
    UObject::PostLoad();
    auto* Property = CastFieldChecked<FArrayProperty>(GetClass()->FindPropertyByName(FName(DataEntriesProperty)));
    const auto *StructProperty = CastFieldChecked<FStructProperty>(Property->Inner);
    check(StructProperty->Struct == GetEntryStruct());

    auto &Array = Property->GetPropertyValue_InContainer(this);
    GameDataEntries = MakeUnique<FScriptArrayHelper>(Property, &Array);
    IDProperty = CastFieldChecked<FNameProperty>(StructProperty->Struct->FindPropertyByName(FName(EntryIDProperty)));
    RowIndexProperty = CastFieldChecked<FIntProperty>(StructProperty->Struct->FindPropertyByName(FName(EntryRowIndexProperty)));
    
    RebuildIndices();
}

#if WITH_EDITOR
void UGameDataRepository::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    UObject::PostEditChangeProperty(PropertyChangedEvent);
    RebuildIndices();
}
#endif

void UGameDataRepository::RebuildIndices()
{
    RowIndices.Empty();
    for (int32 i = 0; i < GameDataEntries->Num(); i++)
    {
        auto *Entry = GameDataEntries->GetElementPtr(i);
        RowIndexProperty->SetValue_InContainer(Entry, i);
        RowIndices.Add(IDProperty->GetPropertyValue_InContainer(Entry), i);
    }
}
