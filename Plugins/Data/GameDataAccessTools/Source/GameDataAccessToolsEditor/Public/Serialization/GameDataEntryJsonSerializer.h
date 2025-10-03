// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameDataEntrySerializer.h"
#include "UObject/Object.h"

#include "GameDataEntryJsonSerializer.generated.h"

/**
 * @class UGameDataEntryJsonSerializer
 * @brief Provides functionality to serialize and deserialize game data entries in JSON format.
 *
 * This class extends the UGameDataEntrySerializer base class and offers methods for
 * handling the JSON serialization and deserialization of game data repositories. It supports
 * specific repository classes, retrieves file extension information, and identifies the format name.
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntryJsonSerializer : public UGameDataEntrySerializer
{
    GENERATED_BODY()

  public:
    bool Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass) override;
    FString GetFileExtensionText() const override;
    FText GetFormatName() const override;
    bool Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository,
                   FString &ErrorMessage) const override;
    bool Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository,
                     FString &ErrorMessage) const override;
};
