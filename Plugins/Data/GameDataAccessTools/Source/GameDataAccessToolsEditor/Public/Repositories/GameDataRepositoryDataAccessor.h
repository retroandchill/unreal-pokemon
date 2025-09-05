// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

class UGameDataRepository;

struct FEntryRowData
{
    int32 Index;
    FName Id;
    TSharedPtr<FStructOnScope> Entry;

    FEntryRowData(const int32 InIndex, const FName InName, TSharedPtr<FStructOnScope> InEntry)
        : Index(InIndex), Id(InName), Entry(MoveTemp(InEntry))
    {
    }

    bool operator==(const FEntryRowData& Other) const = default;
};

/**
 * 
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataRepositoryDataAccessor
{
public:
    explicit FGameDataRepositoryDataAccessor(UGameDataRepository* Repository);

    UGameDataRepository* GetGameDataRepository() const
    {
        return GameDataRepository.Get();
    }

    FArrayProperty* GetGameDataEntriesProperty() const
    {
        return GameDataEntriesProperty;   
    }

    int32 GetNumEntries() const;

    TSharedPtr<FStructOnScope> GetEntryAtIndex(int32 Index);

    uint8* GetEntryAtIndexPtr(int32 Index);

    TSharedPtr<FStructOnScope> GetFirstEntry();

    TArray<TSharedPtr<FEntryRowData>> GetEntries();
    
    TOptional<FName> GenerateUniqueRowName();
    
    bool VerifyRowNameUnique(FName Name);

    void MoveEntries(FScriptArray& Entries);

    void AddEntry(FName Name = NAME_None);
    
    void SwapEntries(int32 Index1, int32 Index2);

    void RemoveEntry(int32 Index);

    void ClearEntries(int32 Slack = 0);

    void RefreshRows();

    FName GetID(int32 Index);

    void SetID(int32 Index, FName Id);

private:
    FName GetID(const uint8* Entry) const;
    void SetID(uint8* Entry, const FName Id) const;
    void SetRowIndex(uint8* Entry, const int32 Id) const;
    
    TObjectPtr<UGameDataRepository> GameDataRepository;
    TObjectPtr<const UScriptStruct> GameDataEntryStruct;
    FArrayProperty* GameDataEntriesProperty;
    FScriptArrayHelper GameDataEntries;
    FNameProperty* IdProperty;
    FIntProperty* RowIndexProperty;
};
