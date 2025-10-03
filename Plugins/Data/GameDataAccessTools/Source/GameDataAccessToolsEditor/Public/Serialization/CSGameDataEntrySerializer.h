// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/GameDataEntrySerializer.h"

#include "CSGameDataEntrySerializer.generated.h"

/**
 * @class UCSGameDataEntrySerializer
 * @brief A specialized serializer for handling game data entries in a specific format.
 *
 * This class extends UGameDataEntrySerializer and provides specific functionality
 * for serializing and deserializing game data entries using a customizable implementation
 * that is partially exposed to Blueprints.
 *
 * The serializer allows the handling of specific repositories and formats, providing
 * an extension point for Blueprint integration through `BlueprintImplementableEvent` methods.
 *
 * The core functionality includes support checking, retrieval of format details, and actual
 * serialization/deserialization behaviors.
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UCSGameDataEntrySerializer : public UGameDataEntrySerializer
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

  protected:
    /**
     * @brief Blueprint-exposed function to check if a specific repository class is supported by the serializer.
     *
     * This function serves as a Blueprint-implementable extension point for determining whether the
     * provided repository class is compatible with this serializer. The functionality is delegated
     * to the Blueprint implementation.
     *
     * @param RepositoryClass The class type of the repository to check for support.
     * @return true if the specified repository class is supported; otherwise, false.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta = (ScriptName = Supports))
    bool K2_Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass);

    /**
     * @brief Retrieves the name of the serialization format.
     *
     * This function provides the format name for the specific serializer. It is intended
     * to be implemented in Blueprints and serve as a customizable extension point. The
     * name helps identify the specific serialization logic associated with this instance.
     *
     * @return A localized text object representing the name of the serialization format.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta = (ScriptName = GetFormatName))
    FText K2_GetFormatName() const;

    /**
     * @brief Retrieves the file extension text associated with the serializer.
     *
     * This function is a Blueprint-implementable hook that provides the file extension
     * text used by this serializer. It is intended to be implemented in Blueprints to
     * define the specific file extension text relevant to a custom serialization format.
     *
     * @return A string representing the file extension for this serializer.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta = (ScriptName = GetFileExtensionText))
    FString K2_GetFileExtensionText() const;

    /**
     * @brief Blueprint-implementable event to serialize game data to a file.
     *
     * This function is a customizable entry point for serializing game data from a repository
     * into a file. It is intended to be implemented in Blueprints and allows users to define
     * the specific logic required for serialization in a custom format. The function leverages
     * the provided file path, data repository, and outputs an error message in case of failure.
     *
     * @param FilePath The path of the file to which game data will be serialized.
     * @param Repository The asset game data repository containing the data to be serialized.
     * @param ErrorMessage An output string that will contain an error message if serialization fails.
     * @return true if serialization succeeds; otherwise, false.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta = (ScriptName = Serialize))
    bool K2_Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository, FString &ErrorMessage) const;

    /**
     * @brief Blueprint-implementable event to deserialize game data from a file into a repository.
     *
     * This function acts as a Blueprint customization point, allowing users to implement
     * specific deserialization logic for reading game data from a file and populating an
     * asset game data repository. It provides flexibility for handling custom formats and
     * error management through Blueprint scripting.
     *
     * @param FilePath The path of the file from which game data will be deserialized.
     * @param Repository The asset game data repository that will be populated with the deserialized data.
     * @param ErrorMessage An output string that will contain an error message if deserialization fails.
     * @return true if deserialization is successful; otherwise, false.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta = (ScriptName = Deserialize))
    bool K2_Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository, FString &ErrorMessage) const;
};
