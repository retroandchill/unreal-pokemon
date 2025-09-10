// "Unreal Pokémon" created by Retro & Chill.


#include "Serialization/CSGameDataEntrySerializer.h"

bool UCSGameDataEntrySerializer::Supports(const TSubclassOf<UAssetGameDataRepository> RepositoryClass)
{
    return K2_Supports(RepositoryClass);
}

FString UCSGameDataEntrySerializer::GetFileExtensionText() const
{
    return K2_GetFileExtensionText();
}

FText UCSGameDataEntrySerializer::GetFormatName() const
{
    return K2_GetFormatName();
}

bool UCSGameDataEntrySerializer::Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository,
    FString &ErrorMessage) const
{
    return K2_Serialize(FilePath, Repository, ErrorMessage);
}

bool UCSGameDataEntrySerializer::Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository,
    FString &ErrorMessage) const
{
    return K2_Deserialize(FilePath, Repository, ErrorMessage);
}