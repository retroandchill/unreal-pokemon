// "Unreal Pokémon" created by Retro & Chill.


#include "Serialization/GameDataEntryJsonSerializer.h"
#include "GameDataRepository.h"
#include "JsonObjectConverter.h"
#include "Repositories/GameDataRepositoryDataAccessor.h"

struct FScopedArrayDeleter
{
    explicit FScopedArrayDeleter(FScriptArrayHelper& Helper)
        : Helper(Helper)
    {
    }

    ~FScopedArrayDeleter()
    {
        Helper.DestroyContainer_Unsafe();
    }

    UE_NONCOPYABLE(FScopedArrayDeleter)

private:
    FScriptArrayHelper& Helper;
};

bool UGameDataEntryJsonSerializer::Supports_Implementation(TSubclassOf<UGameDataRepository> RepositoryClass)
{
    return true;
}

FString UGameDataEntryJsonSerializer::GetFileExtensionText_Implementation() const
{
    return TEXT("JSON file |*.json|");
}

FText UGameDataEntryJsonSerializer::GetFormatName_Implementation() const
{
    return NSLOCTEXT("GameDataEntryJsonSerializer", "FormatName", "JSON");
}

bool UGameDataEntryJsonSerializer::Serialize_Implementation(const FString &FilePath,
                                                            const UGameDataRepository *Repository,
                                                            FString &ErrorMessage) const
{
    FGameDataRepositoryDataAccessor DataAccessor(const_cast<UGameDataRepository *>(Repository));

    TArray<TSharedPtr<FJsonValue>> JsonValues;
    for (auto Row : DataAccessor.GetEntries())
    {
        auto JsonObject = MakeShared<FJsonObject>();
        if (!FJsonObjectConverter::UStructToJsonObject(Row->Entry->GetStruct(), Row->Entry->GetStructMemory(), JsonObject))
        {
            ErrorMessage = TEXT("Failed to serialize entry");
            return false;
        }

        JsonValues.Emplace(MakeShared<FJsonValueObject>(JsonObject));
    }

    FString Result;
    const auto Writer = TJsonWriterFactory<>::Create(&Result);
    if (!FJsonSerializer::Serialize(JsonValues, Writer))
    {
        ErrorMessage = TEXT("Failed to serialize entries");
        return false;   
    }

    if (!FFileHelper::SaveStringToFile(Result, *FilePath))
    {
        ErrorMessage = TEXT("Failed to save file");
        return false;  
    }
    
    return true;
}

bool UGameDataEntryJsonSerializer::Deserialize_Implementation(const FString &FilePath, UGameDataRepository *Repository,
    FString &ErrorMessage) const
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        ErrorMessage = TEXT("Failed to load file");
        return false;   
    }

    TArray<TSharedPtr<FJsonValue>> JsonValues;
    if (const auto Reader = TJsonReaderFactory<>::Create(MoveTemp(JsonString)); !FJsonSerializer::Deserialize(Reader, JsonValues))
    {
        ErrorMessage = TEXT("Failed to deserialize entries");
        return false; 
    }

    
    FGameDataRepositoryDataAccessor DataAccessor(Repository);
    FScriptArray StagingArray;
    FScriptArrayHelper ArrayHelper(DataAccessor.GetGameDataEntriesProperty(), &StagingArray);
    FScopedArrayDeleter Deleter(ArrayHelper);
    auto *StructType = DataAccessor.GetGameDataRepository()->GetEntryStruct();

    for (const auto &JsonValue : JsonValues)
    {
        auto JsonObject = JsonValue->AsObject();
        if (!JsonObject.IsValid())
        {
            ErrorMessage = TEXT("Failed to deserialize entry");
            return false;
        }

        if (const int32 NewIndex = ArrayHelper.AddValue(); !FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructType, ArrayHelper.GetElementPtr(NewIndex)))
        {
            ErrorMessage = TEXT("Failed to deserialize entry");
            return false;
        }
    }

    DataAccessor.MoveEntries(StagingArray);
    return true;
}