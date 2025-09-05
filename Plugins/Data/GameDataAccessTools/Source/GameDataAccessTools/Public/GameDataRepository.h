// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameDataRepository.generated.h"

/**
 *
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UGameDataRepository : public UObject
{
    GENERATED_BODY()

public:
    static constexpr FStringView DataEntriesProperty = TEXT("DataEntries");
    static constexpr FStringView EntryIDProperty = TEXT("ID");
    static constexpr FStringView EntryRowIndexProperty = TEXT("RowIndex");
    
    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataRepository")
    const UScriptStruct* GetEntryStruct() const;

    UFUNCTION(BlueprintPure, Category = "GameDataRepository")
    int32 GetNumEntries() const;

    uint8* GetEntryAtIndex(int32 Index) const;
    
protected:
    void PostLoad() override;
    
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif


private:
    void RebuildIndices();
    
#if WITH_EDITOR
    friend class FGameDataRepositoryDataAccessor;
#endif

    TUniquePtr<FScriptArrayHelper> GameDataEntries;
    FNameProperty* IDProperty = nullptr;
    FIntProperty* RowIndexProperty = nullptr;
    TMap<FName, int32> RowIndices;
};
