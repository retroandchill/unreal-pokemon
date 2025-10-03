// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameDataEntrySerializer.generated.h"

class UAssetGameDataRepository;

/**
 * @class UGameDataEntrySerializer
 * @brief An abstract base class for serializing and deserializing game data entries.
 *
 * This class provides functionality to manage game data repository entries through serialization
 * and deserialization processes, as well as specifying custom file formats and repository compatibility.
 *
 * It is intended to be subclassed for implementing specific serializers.
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntrySerializer : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Checks if the serializer supports the specified repository class.
     *
     * This method determines whether a given repository class is compatible with the current serializer.
     * It is intended to be implemented in derived classes to define support criteria.
     *
     * @param RepositoryClass The class type of the repository to check for compatibility.
     * @return True if the serializer supports the specified repository class, otherwise false.
     */
    virtual bool Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass);

    /**
     * @brief Retrieves the name of the file format associated with the serializer.
     *
     * This method is intended to be overridden in subclasses to provide the display name
     * of the file format that the serializer supports. The format name is used for user-facing
     * interactions, such as menu entries for import/export actions.
     *
     * @return A localized text representing the name of the supported file format.
     */
    virtual FText GetFormatName() const;

    /**
     * @brief Retrieves the file extension associated with the serializer.
     *
     * This method is intended to be overridden in subclasses to provide the file extension
     * format used by the serializer. It is utilized in file operations such as import/export
     * for identifying compatible file types.
     *
     * @return A string representing the file extension supported by the serializer.
     */
    virtual FString GetFileExtensionText() const;

    /**
     * @brief Serializes the data from the specified repository to a file.
     *
     * This method performs the serialization process for saving the contents
     * of a given game data repository to a file at a specified path. The implementation
     * should be provided in derived classes to define the specific serialization logic.
     *
     * @param FilePath The path of the file where the repository data will be serialized.
     * @param Repository A pointer to the repository containing the game data to serialize.
     * @param ErrorMessage A reference to a string that will store an error message if the serialization fails.
     * @return True if serialization is successful, otherwise false.
     */
    virtual bool Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository,
                           FString &ErrorMessage) const;

    /**
     * @brief Deserializes the data from a file into the specified game data repository.
     *
     * This method performs the deserialization process for importing the contents of
     * a file at the specified path into a given game data repository. The actual deserialization
     * logic should be implemented in derived classes. If the operation fails, an error message
     * will be provided.
     *
     * @param FilePath The path of the file to deserialize data from.
     * @param Repository A pointer to the repository where the deserialized game data will be stored.
     * @param ErrorMessage A reference to a string that will store an error message if the deserialization fails.
     * @return True if deserialization is successful, otherwise false.
     */
    virtual bool Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository,
                             FString &ErrorMessage) const;

  protected:
    /**
     * @brief Retrieves the array of game data entries from the specified repository.
     *
     * This method accesses the game data entries stored within the provided game data repository,
     * enabling further processing or iteration over the entries.
     *
     * @param Repository A pointer to the UAssetGameDataRepository instance from which the game data entries are
     * retrieved.
     * @return A reference to an FScriptArrayHelper object that encapsulates the game data entries in the repository.
     */
    static FScriptArrayHelper &GetGameDataEntries(const UAssetGameDataRepository *Repository);

    /**
     * @brief Retrieves the array property of game data entries from the specified repository.
     *
     * This method provides access to the property representing game data entries within the
     * provided repository. It is used to interact with the array of entries managed by the repository.
     *
     * @param Repository A pointer to the UAssetGameDataRepository instance containing the game data entries property.
     * @return A pointer to an FArrayProperty object representing the game data entries in the repository.
     */
    static FArrayProperty *GetGameDataEntriesProperty(const UAssetGameDataRepository *Repository);

    /**
     * @brief Transfers game data entries from the given array to the repository.
     *
     * This method moves the entries specified in the provided array to the target game data repository,
     * ensuring that the entries are relocated without creating duplicates.
     *
     * @param Repository A pointer to the target game data repository where the entries will be moved.
     * @param Entries A reference to the array containing the entries to be moved to the repository.
     */
    static void MoveEntries(UAssetGameDataRepository *Repository, FScriptArray &Entries);

  private:
    friend class UGameDataEntrySerializerExporter;
};