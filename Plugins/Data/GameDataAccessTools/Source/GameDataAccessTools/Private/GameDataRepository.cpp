#include "GameDataRepository.h"

#include "LogGameDataAccessTools.h"

void UGameDataRepositoryBase::PostLoad()
{
	UObject::PostLoad();
	GameDataEntriesProperty = CastField<FArrayProperty>(GetClass()->FindPropertyByName(FName(DataEntriesProperty)));
	if (GameDataEntriesProperty == nullptr)
	{
		UE_LOG(LogGameDataAccessTools, Error, TEXT("Class %s must have an Array Property named %s"),
		       *GetClass()->GetName(), DataEntriesProperty.GetData());
	}

	StructProperty = CastFieldChecked<FStructProperty>(GameDataEntriesProperty->Inner);
	if (StructProperty == nullptr)
	{
		UE_LOG(LogGameDataAccessTools, Error, TEXT("Array Property %s must have a Struct Property"),
		       *GameDataEntriesProperty->GetName());
	}

	auto& Array = GameDataEntriesProperty->GetPropertyValue_InContainer(this);
	GameDataEntries = MakeUnique<FScriptArrayHelper>(GameDataEntriesProperty, &Array);

	IDProperty = CastFieldChecked<FNameProperty>(StructProperty->Struct->FindPropertyByName(FName(EntryIDProperty)));
	if (IDProperty == nullptr)
	{
		UE_LOG(LogGameDataAccessTools, Error, TEXT("Struct Property %s must have a Name Property named %s"),
		       *StructProperty->GetName(), EntryIDProperty.GetData());
	}

	RowIndexProperty = CastFieldChecked<FIntProperty>(
		StructProperty->Struct->FindPropertyByName(FName(EntryRowIndexProperty)));
	if (RowIndexProperty == nullptr)
	{
		UE_LOG(LogGameDataAccessTools, Error, TEXT("Struct Property %s must have an Int Property named %s"),
		       *StructProperty->GetName(), EntryRowIndexProperty.GetData());
	}

	RebuildIndices();
}

uint8* UGameDataRepositoryBase::AddNewEntry(const FName ID)
{
	if (!ensureMsgf(!ID.IsNone(), TEXT("ID for a row may not be empty")))
	{
		return nullptr;
	}

	if (!VerifyRowNameUnique(ID))
	{
		UE_LOG(LogGameDataAccessTools, Error, TEXT("Cannot use ID '%s', as it is already in use"), *ID.ToString())
		return nullptr;
	}

	const int32 NewIndex = GameDataEntries->AddValue();
	auto* Entry = GameDataEntries->GetElementPtr(NewIndex);
	IDProperty->SetValue_InContainer(Entry, ID);
	RowIndexProperty->SetPropertyValue_InContainer(Entry, NewIndex);
	RowIndices.Add(ID, NewIndex);
	return Entry;
}

void UGameDataRepositoryBase::RemoveEntryAtIndex(const int32 Index)
{
	if (!ensureMsgf(GameDataEntries->IsValidIndex(Index), TEXT("Invalid index %d"), Index))
	{
		return;
	}

	const auto* CurrentEntry = GameDataEntries->GetElementPtr(Index);
	RowIndices.Remove(IDProperty->GetPropertyValue_InContainer(CurrentEntry));
	GameDataEntries->RemoveValues(Index);

	for (int32 i = Index; i < GameDataEntries->Num(); i++)
	{
		auto* Entry = GameDataEntries->GetElementPtr(i);
		RowIndexProperty->SetPropertyValue_InContainer(Entry, i);
		RowIndices[IDProperty->GetPropertyValue_InContainer(Entry)] = i;
	}
}

void UGameDataRepositoryBase::SwapEntries(const int32 Index1, const int32 Index2)
{
	if (!ensureMsgf(GameDataEntries->IsValidIndex(Index1) && GameDataEntries->IsValidIndex(Index2),
	                TEXT("Invalid index %d or %d"), Index1, Index2))
	{
		return;
	}

	auto* Entry1 = GameDataEntries->GetElementPtr(Index1);
	auto* Entry2 = GameDataEntries->GetElementPtr(Index2);
	GameDataEntries->SwapValues(Index1, Index2);

	RowIndexProperty->SetPropertyValue_InContainer(Entry1, Index2);
	RowIndices[IDProperty->GetPropertyValue_InContainer(Entry1)] = Index2;

	RowIndexProperty->SetPropertyValue_InContainer(Entry2, Index1);
	RowIndices[IDProperty->GetPropertyValue_InContainer(Entry2)] = Index1;
}

void UGameDataRepositoryBase::RebuildIndices()
{
	RowIndices.Empty();
	for (int32 i = 0; i < GameDataEntries->Num(); i++)
	{
		auto* Entry = GameDataEntries->GetElementPtr(i);
		RowIndexProperty->SetValue_InContainer(Entry, i);
		RowIndices.Add(IDProperty->GetPropertyValue_InContainer(Entry), i);
	}
}

bool UGameDataRepositoryBase::VerifyRowNameUnique(const FName Name) const
{
	bool bFirstInstance = false;
	for (int32 i = 0; i < GameDataEntries->Num(); i++)
	{
		if (const auto* Entry = GameDataEntries->GetElementPtr(i); IDProperty->GetPropertyValue_InContainer(Entry) ==
			Name)
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

TOptional<FName> UGameDataRepositoryBase::GenerateUniqueRowName() const
{
	TSet<FName> UsedNames;
	for (int32 i = 0; i < GameDataEntries->Num(); i++)
	{
		const auto* Entry = GameDataEntries->GetElementPtr(i);
		UsedNames.Add(IDProperty->GetPropertyValue_InContainer(Entry));
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
