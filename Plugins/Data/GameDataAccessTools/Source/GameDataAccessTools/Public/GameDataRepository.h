// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameDataRepository.generated.h"

UCLASS(Abstract, NotBlueprintable)
class GAMEDATAACCESSTOOLS_API UGameDataRepositoryBase : public UObject
{
	GENERATED_BODY()
	
public:
	static constexpr FStringView DataEntriesProperty = TEXT("DataEntries");
	static constexpr FStringView EntryIDProperty = TEXT("ID");
	static constexpr FStringView EntryRowIndexProperty = TEXT("RowIndex");

	UFUNCTION(BlueprintPure, Category = "GameDataRepository")
	const UScriptStruct* GetEntryStruct() const
	{
		return StructProperty->Struct;
	}

	UFUNCTION(BlueprintPure, Category = "GameDataRepository")
	int32 GetNumEntries() const
	{
		return GameDataEntries->Num();
	}

	const uint8* GetEntryAtIndex(const int32 Index) const
	{
		return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
	}

	
#if WITH_EDITOR
	uint8* GetMutableEntryAtIndex(const int32 Index) const
	{
		return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
	}
#endif
    
protected:
	void PostLoad() override;

	uint8* AddNewEntry(FName ID);

	void RemoveEntryAtIndex(int32 Index);

	void SwapEntries(int32 Index1, int32 Index2);

	void RebuildIndices();
	
private:
	bool VerifyRowNameUnique(FName Name) const;

	TOptional<FName> GenerateUniqueRowName() const;
	
#if WITH_EDITOR
	friend class FGameDataRepositoryEditor;
	friend class UGameDataEntrySerializer;
#endif

	TUniquePtr<FScriptArrayHelper> GameDataEntries;
	FArrayProperty* GameDataEntriesProperty = nullptr;
	FStructProperty* StructProperty = nullptr;
	FNameProperty* IDProperty = nullptr;
	FIntProperty* RowIndexProperty = nullptr;
	TMap<FName, int32> RowIndices;
};

/**
 *
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UGameDataRepository : public UGameDataRepositoryBase
{
    GENERATED_BODY()

public:
	
};
