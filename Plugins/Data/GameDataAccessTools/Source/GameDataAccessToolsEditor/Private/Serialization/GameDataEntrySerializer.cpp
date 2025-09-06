
#include "Serialization/GameDataEntrySerializer.h"
#include "GameDataRepository.h"

bool UGameDataEntrySerializer::Supports_Implementation(TSubclassOf<UAssetGameDataRepository> RepositoryClass)
{
    PURE_VIRTUAL(UGameDataEntrySerializer::Supports_Implementation);
    return false;
}

FText UGameDataEntrySerializer::GetFormatName_Implementation() const
{
    PURE_VIRTUAL(UGameDataEntrySerializer::GetFormatName_Implementation);
    return FText::GetEmpty();
}

FString UGameDataEntrySerializer::GetFileExtensionText_Implementation() const
{
    PURE_VIRTUAL(UGameDataEntrySerializer::GetFileExtensionText_Implementation);
    return TEXT("");
}

bool UGameDataEntrySerializer::Serialize_Implementation(const FString &FilePath,
                                                        const UAssetGameDataRepository *Repository,
                                                        FString &ErrorMessage) const
{
    PURE_VIRTUAL(UGameDataEntrySerializer::Serialize_Implementation);
    return false;
}

bool UGameDataEntrySerializer::Deserialize_Implementation(const FString &FilePath, UAssetGameDataRepository *Repository,
                                                          FString &ErrorMessage) const
{
    PURE_VIRTUAL(UGameDataEntrySerializer::Deserialize_Implementation);
    return false;
}

FScriptArrayHelper &UGameDataEntrySerializer::GetGameDataEntries(const UAssetGameDataRepository *Repository)
{
    return *Repository->GameDataEntries.Get();
}

FArrayProperty *UGameDataEntrySerializer::GetGameDataEntriesProperty(const UAssetGameDataRepository *Repository)
{
    return Repository->GameDataEntriesProperty;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UGameDataEntrySerializer::MoveEntries(UAssetGameDataRepository *Repository, FScriptArray &Entries)
{
    Repository->GameDataEntries->MoveAssign(&Entries);
}
