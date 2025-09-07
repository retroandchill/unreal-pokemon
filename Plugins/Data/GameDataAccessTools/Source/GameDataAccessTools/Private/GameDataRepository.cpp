#include "GameDataRepository.h"
#include "LogGameDataAccessTools.h"

void UGameDataRepository::PostInitProperties()
{
    UObject::PostInitProperties();
    if (GetClass()->HasAnyClassFlags(CLASS_Abstract))
    {
        return;
    }

    GameDataEntriesProperty = CastField<FArrayProperty>(GetClass()->FindPropertyByName(FName(DataEntriesProperty)));
    if (GameDataEntriesProperty == nullptr)
    {
        UE_LOG(LogGameDataAccessTools, Error, TEXT("Class %s must have an Array Property named %s"),
               *GetClass()->GetName(), DataEntriesProperty.GetData());
        return;
    }

    StructProperty = CastFieldChecked<FStructProperty>(GameDataEntriesProperty->Inner);
    if (StructProperty == nullptr)
    {
        UE_LOG(LogGameDataAccessTools, Error, TEXT("Array Property %s must have a Struct Property"),
               *GameDataEntriesProperty->GetName());
        return;
    }

    DataArray = &GameDataEntriesProperty->GetPropertyValue_InContainer(this);
    GameDataEntries = MakeUnique<FScriptArrayHelper>(GameDataEntriesProperty, DataArray);

    IDProperty = CastFieldChecked<FNameProperty>(StructProperty->Struct->FindPropertyByName(FName(EntryIDProperty)));
    if (IDProperty == nullptr)
    {
        UE_LOG(LogGameDataAccessTools, Error, TEXT("Struct Property %s must have a Name Property named %s"),
               *StructProperty->GetName(), EntryIDProperty.GetData());
    }

    RowIndexProperty =
        CastFieldChecked<FIntProperty>(StructProperty->Struct->FindPropertyByName(FName(EntryRowIndexProperty)));
    if (RowIndexProperty == nullptr)
    {
        UE_LOG(LogGameDataAccessTools, Error, TEXT("Struct Property %s must have an Int Property named %s"),
               *StructProperty->GetName(), EntryRowIndexProperty.GetData());
    }
}

void UGameDataRepository::PostLoad()
{
    UObject::PostLoad();
    RebuildIndices();
}

uint8 *UGameDataRepository::AddNewEntry(const FName ID)
{
    auto *Entry = AddNewEntryInternal(ID);
    const int32 NewIndex = GameDataEntries->Num() - 1;
    IDProperty->SetValue_InContainer(Entry, ID);
    RowIndexProperty->SetPropertyValue_InContainer(Entry, NewIndex);
    RowIndices.Add(ID, NewIndex);
    return Entry;
}

uint8 *UGameDataRepository::AddNewEntry(const FName ID, const uint8 *Data)
{
    auto *Entry = AddNewEntryInternal(ID);
    const int32 NewIndex = GameDataEntries->Num() - 1;
    StructProperty->Struct->CopyScriptStruct(Entry, Data);
    IDProperty->SetValue_InContainer(Entry, ID);
    RowIndexProperty->SetPropertyValue_InContainer(Entry, NewIndex);
    RowIndices.Add(ID, NewIndex);
    return Entry;
}

// ReSharper disable once CppMemberFunctionMayBeConst
uint8 *UGameDataRepository::AddNewEntryInternal(const FName ID)
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
    return GameDataEntries->GetElementPtr(NewIndex);
}

void UGameDataRepository::RemoveEntryAtIndex(const int32 Index)
{
    if (!ensureMsgf(GameDataEntries->IsValidIndex(Index), TEXT("Invalid index %d"), Index))
    {
        return;
    }

    const auto *CurrentEntry = GameDataEntries->GetElementPtr(Index);
    RowIndices.Remove(IDProperty->GetPropertyValue_InContainer(CurrentEntry));
    GameDataEntries->RemoveValues(Index);

    for (int32 i = Index; i < GameDataEntries->Num(); i++)
    {
        auto *Entry = GameDataEntries->GetElementPtr(i);
        RowIndexProperty->SetPropertyValue_InContainer(Entry, i);
        RowIndices[IDProperty->GetPropertyValue_InContainer(Entry)] = i;
    }
}

void UGameDataRepository::SwapEntries(const int32 Index1, const int32 Index2)
{
    if (!ensureMsgf(GameDataEntries->IsValidIndex(Index1) && GameDataEntries->IsValidIndex(Index2),
                    TEXT("Invalid index %d or %d"), Index1, Index2))
    {
        return;
    }

    auto *Entry1 = GameDataEntries->GetElementPtr(Index1);
    auto *Entry2 = GameDataEntries->GetElementPtr(Index2);
    GameDataEntries->SwapValues(Index1, Index2);

    RowIndexProperty->SetPropertyValue_InContainer(Entry1, Index2);
    RowIndices[IDProperty->GetPropertyValue_InContainer(Entry1)] = Index2;

    RowIndexProperty->SetPropertyValue_InContainer(Entry2, Index1);
    RowIndices[IDProperty->GetPropertyValue_InContainer(Entry2)] = Index1;
}

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

TOptional<int32> UGameDataRepository::GetRowIndex(const FName ID) const
{
    auto *Index = RowIndices.Find(ID);
    return Index != nullptr ? *Index : TOptional<int32>();
}

bool UGameDataRepository::VerifyRowNameUnique(const FName Name) const
{
    bool bFirstInstance = false;
    for (int32 i = 0; i < GameDataEntries->Num(); i++)
    {
        if (const auto *Entry = GameDataEntries->GetElementPtr(i);
            IDProperty->GetPropertyValue_InContainer(Entry) == Name)
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

TOptional<FName> UGameDataRepository::GenerateUniqueRowName() const
{
    TSet<FName> UsedNames;
    for (int32 i = 0; i < GameDataEntries->Num(); i++)
    {
        const auto *Entry = GameDataEntries->GetElementPtr(i);
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

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UStaticGameDataRepository::TryRegisterEntryInternal(const int32 &)
{
    checkf(false, TEXT("This should never get called"));
    return false;
}

DEFINE_FUNCTION(UStaticGameDataRepository::execTryRegisterEntryInternal)
{
    auto *Self = CastChecked<UStaticGameDataRepository>(Context);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto DataStructProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    const auto *DataStructStruct = Stack.MostRecentPropertyAddress;

    if (DataStructProperty->Struct != Self->GetEntryStruct())
    {
        UE_LOG(LogGameDataAccessTools, Fatal, TEXT("Provided struct type does not match the struct of the class"));
    }

    P_FINISH;

    P_NATIVE_BEGIN

    const auto ID = Self->GetIDProperty()->GetPropertyValue_InContainer(DataStructStruct);
    auto *NewEntry = Self->AddNewEntry(ID);
    Self->GetEntryStruct()->CopyScriptStruct(NewEntry, DataStructStruct);
    *static_cast<bool *>(RESULT_PARAM) = true;

    P_NATIVE_END
}

bool UStaticGameDataRepository::TryUnregisterEntry(const FName ID)
{
    auto Index = GetRowIndex(ID);
    if (!Index.IsSet())
    {
        return false;
    }

    RemoveEntryAtIndex(Index.GetValue());
    return true;
}
