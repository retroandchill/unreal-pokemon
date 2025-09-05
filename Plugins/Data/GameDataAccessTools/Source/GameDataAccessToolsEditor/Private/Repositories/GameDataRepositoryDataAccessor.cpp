// "Unreal Pokémon" created by Retro & Chill.


#include "Repositories/GameDataRepositoryDataAccessor.h"
#include "GameDataRepository.h"


FGameDataRepositoryDataAccessor::FGameDataRepositoryDataAccessor(UGameDataRepository *Repository) : GameDataRepository(Repository), GameDataEntryStruct(Repository->GetEntryStruct()), GameDataEntriesProperty(CastFieldChecked<FArrayProperty>(GameDataRepository->GetClass()->FindPropertyByName(TEXT("DataEntries")))), GameDataEntries(GameDataEntriesProperty, GameDataEntriesProperty->ContainerPtrToValuePtr<TArray<UObject*>>(GameDataRepository)), IdProperty(CastFieldChecked<FNameProperty>(GameDataEntryStruct->FindPropertyByName(TEXT("ID")))), RowIndexProperty(CastFieldChecked<FIntProperty>(GameDataEntryStruct->FindPropertyByName(TEXT("RowIndex"))))
{
}

int32 FGameDataRepositoryDataAccessor::GetNumEntries() const
{
    return GameDataEntries.Num();
}

TSharedPtr<FStructOnScope> FGameDataRepositoryDataAccessor::GetEntryAtIndex(const int32 Index)
{
    return GameDataEntries.IsValidIndex(Index) ? MakeShared<FStructOnScope>(GameDataEntryStruct, GameDataEntries.GetElementPtr(Index)) : TSharedPtr<FStructOnScope>();   
}

uint8 * FGameDataRepositoryDataAccessor::GetEntryAtIndexPtr(const int32 Index)
{
    return GameDataEntries.IsValidIndex(Index) ? GameDataEntries.GetElementPtr(Index) : nullptr;  
}

TSharedPtr<FStructOnScope> FGameDataRepositoryDataAccessor::GetFirstEntry()
{
    return GetEntryAtIndex(0);
}

TArray<TSharedPtr<FEntryRowData>> FGameDataRepositoryDataAccessor::GetEntries()
{
    TArray<TSharedPtr<FEntryRowData>> Entries;
    for (int32 i = 0; i < GameDataEntries.Num(); i++)
    {
        
        auto *Entry = GameDataEntries.GetElementPtr(i);
        Entries.Emplace(MakeShared<FEntryRowData>(i, GetID(Entry), MakeShared<FStructOnScope>(GameDataEntryStruct, Entry)));
    }
    return Entries;
}

TOptional<FName> FGameDataRepositoryDataAccessor::GenerateUniqueRowName()
{
    TSet<FName> UsedNames;
    for (int32 i = 0; i < GameDataEntries.Num(); i++)
    {
        const auto *Entry = GameDataEntries.GetElementPtr(i);
        UsedNames.Add(GetID(Entry));
    }

    static FName StartingEntry = TEXT("Entry");
    FName NewName = StartingEntry;
    int32 Index = 1;
    while (UsedNames.Contains(NewName))
    {
        NewName = FName(*FString::Printf(TEXT("%s%d"), *StartingEntry.ToString(), Index));

        if (Index == std::numeric_limits<int32>::max())
        {
            return TOptional<FName>();
        }
        Index++;
    }

    return NewName;   
}

bool FGameDataRepositoryDataAccessor::VerifyRowNameUnique(const FName Name)
{
    bool bFirstInstance = false;
    for (int32 i = 0; i < GameDataEntries.Num(); i++)
    {
        if (const auto *Entry = GameDataEntries.GetElementPtr(i); GetID(Entry) == Name)
        {
            if (!bFirstInstance)
            {
                bFirstInstance = true;
                continue;
            }
            return false;
        }
    }

    return true;
}

void FGameDataRepositoryDataAccessor::MoveEntries(FScriptArray& Entries)
{
    GameDataEntries.MoveAssign(&Entries);
    RefreshRows();
    GameDataRepository->RebuildIndices();
}

void FGameDataRepositoryDataAccessor::AddEntry(FName Name)
{
    if (Name.IsNone())
    {
        Name = GenerateUniqueRowName().GetValue();
    }
    
    const int32 NewIndex = GameDataEntries.AddUninitializedValue();
    auto *NewEntry = GameDataEntries.GetElementPtr(NewIndex);
    GameDataEntryStruct->InitializeStruct(NewEntry);
    SetID(NewEntry, Name);
    SetRowIndex(NewEntry, GameDataEntries.Num());
}

void FGameDataRepositoryDataAccessor::SwapEntries(int32 Index1, int32 Index2)
{
    GameDataEntries.SwapValues(Index1, Index2);   
}

void FGameDataRepositoryDataAccessor::RemoveEntry(const int32 Index)
{
    GameDataEntries.RemoveValues(Index);
}

void FGameDataRepositoryDataAccessor::ClearEntries(const int32 Slack)
{
    GameDataEntries.EmptyValues(Slack);
}

void FGameDataRepositoryDataAccessor::RefreshRows()
{
    for (int32 i = 0; i < GameDataEntries.Num(); i++)
    {
        auto *Entry = GameDataEntries.GetElementPtr(i);
        SetRowIndex(Entry, i);
    }
}

FName FGameDataRepositoryDataAccessor::GetID(const int32 Index)
{
    return GetID(GameDataEntries.GetElementPtr(Index));   
}

void FGameDataRepositoryDataAccessor::SetID(const int32 Index, const FName Id)
{
    SetID(GameDataEntries.GetElementPtr(Index), Id);  
}

FName FGameDataRepositoryDataAccessor::GetID(const uint8 *Entry) const
{
    FName Result;
    IdProperty->GetValue_InContainer(Entry, &Result);
    return Result;
}

void FGameDataRepositoryDataAccessor::SetID(uint8 *Entry, const FName Id) const
{
    IdProperty->SetValue_InContainer(Entry, Id);
}

void FGameDataRepositoryDataAccessor::SetRowIndex(uint8 *Entry, const int32 Id) const
{
    RowIndexProperty->SetPropertyValue_InContainer(Entry, Id);
}