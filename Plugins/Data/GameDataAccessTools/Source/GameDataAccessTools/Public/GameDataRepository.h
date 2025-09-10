// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameDataRepository.generated.h"

UCLASS(Abstract, NotBlueprintable)
class GAMEDATAACCESSTOOLS_API UGameDataRepository : public UObject
{
    GENERATED_BODY()

  public:
    static constexpr FStringView DataEntriesProperty = TEXT("DataEntries");
    static constexpr FStringView EntryIDProperty = TEXT("ID");
    static constexpr FStringView EntryRowIndexProperty = TEXT("RowIndex");

    UFUNCTION(BlueprintPure, Category = "GameDataRepository")
    const UScriptStruct *GetEntryStruct() const
    {
        return StructProperty->Struct;
    }

    UFUNCTION(BlueprintPure, Category = "GameDataRepository")
    int32 GetNumEntries() const
    {
        return GameDataEntries->Num();
    }

    const uint8 *GetEntryByID(const FName ID) const
    {
        auto Index = GetRowIndex(ID);
        return Index.IsSet() ? GameDataEntries->GetElementPtr(Index.GetValue()) : nullptr;
    }

    const uint8 *GetEntryAtIndex(const int32 Index) const
    {
        return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
    }

#if WITH_EDITOR
    uint8 *GetMutableEntryAtIndex(const int32 Index) const
    {
        return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
    }

    bool IsValidRepository() const
    {
        return bValidRepository;
    }
#endif

  protected:
    void PostInitProperties() override;

    void PostLoad() override;

    uint8 *AddNewEntry(FName ID);

    uint8 *AddNewEntry(FName ID, const uint8 *Data);

  private:
    uint8 *AddNewEntryInternal(FName ID);

  protected:
    void RemoveEntryAtIndex(int32 Index);

    void SwapEntries(int32 Index1, int32 Index2);

    void RebuildIndices();

    FNameProperty *GetIDProperty() const
    {
        return IDProperty;
    }

    TOptional<int32> GetRowIndex(FName ID) const;

    bool VerifyRowNameUnique(FName Name) const;

  private:
    TOptional<FName> GenerateUniqueRowName() const;

#if WITH_EDITOR
    friend class FGameDataRepositoryEditor;
    friend class UGameDataEntrySerializer;
#endif
    friend class UGameDataRepositoryExporter;

    const FScriptArray *DataArray = nullptr;
    TUniquePtr<FScriptArrayHelper> GameDataEntries;
    FArrayProperty *GameDataEntriesProperty = nullptr;
    FStructProperty *StructProperty = nullptr;
    FNameProperty *IDProperty = nullptr;
    FIntProperty *RowIndexProperty = nullptr;
    TMap<FName, int32> RowIndices;

#if WITH_EDITOR
    bool bValidRepository = false;
#endif
};

/**
 *
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UAssetGameDataRepository : public UGameDataRepository
{
    GENERATED_BODY()

    
};

UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UStaticGameDataRepository : public UGameDataRepository
{
    GENERATED_BODY()

  protected:
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = DataStruct))
    bool TryRegisterEntryInternal(const int32 &DataStruct, FString& OutError);

    DECLARE_FUNCTION(execTryRegisterEntryInternal);

  public:
    UFUNCTION(meta = (ScriptMethod))
    bool TryUnregisterEntry(FName ID);
};
