// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameDataRepository.generated.h"

/**
 * @class UGameDataRepository
 * @brief Abstract base class for managing game data entries.
 *
 * This class provides functionality to store, retrieve, and manipulate generic
 * game data entries. It is designed to handle structured data defined by a
 * specific data schema or struct.
 *
 * The repository maintains its entries in an internal array and provides features
 * like indexing, ID-based lookups, and runtime validation for data integrity.
 *
 * @note This class is abstract and not intended for direct instantiation.
 * Derived classes should implement specific details as required.
 */
UCLASS(Abstract, NotBlueprintable)
class GAMEDATAACCESSTOOLS_API UGameDataRepository : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @variable DataEntriesProperty
     * @brief Static constant representing the property name for game data entries.
     *
     * This variable is used as a key to reference the array property that stores
     * the structured game data entries within the repository. It serves as an identifier
     * in various operations, ensuring that the correct property is targeted when accessing
     * or manipulating game data.
     *
     * The value of this constant correlates with an array property defined in the class
     * and is critical for operations like initialization, validation, and data retrieval.
     *
     * @note This constant must match the name of the actual corresponding property in subclasses.
     */
    static constexpr FStringView DataEntriesProperty = TEXT("DataEntries");

    /**
     * @variable EntryIDProperty
     * @brief Static constant representing the property name for unique entry identifiers.
     *
     * This variable is used as a key to reference the unique identifier property
     * of each structured game data entry within the repository. It ensures that
     * operations requiring an identifier, such as lookups or validation, target the
     * correct property.
     *
     * The value of this constant is critical for maintaining data integrity and
     * uniquely identifying entries in the repository. It must be defined accurately
     * to match the corresponding property name in the game data structure.
     *
     * @note This constant must align with the actual name of the identifier property
     * in the struct or schema governing data entries.
     */
    static constexpr FStringView EntryIDProperty = TEXT("ID");

    /**
     * @variable EntryRowIndexProperty
     * @brief Static constant representing the property name for entry row indices.
     *
     * This variable is used as a key to reference the property that stores the row index
     * of each structured game data entry within the repository. It ensures that operations
     * requiring positional data, such as sorting or indexing, target the correct property.
     *
     * The value of this constant must align with the name of the corresponding property in the
     * struct or schema governing the data entries. It is critical for enabling repository initialization
     * and validation during runtime.
     *
     * @note This constant must match the actual property name in the underlying data structure
     * to ensure proper functionality and data integrity.
     */
    static constexpr FStringView EntryRowIndexProperty = TEXT("RowIndex");

    /**
     * @brief Retrieves the struct definition used for game data entries in the repository.
     *
     * This function returns a pointer to the script structure (`UScriptStruct`) that defines
     * the schema or layout for individual entries within the data repository. It allows external
     * systems to understand the format of the data stored and perform operations such as validation,
     * serialization, or data manipulation.
     *
     * The struct is defined by a property in the repository, and this function provides runtime
     * access to it for flexible interaction with the repository's data definitions.
     *
     * @return A `UScriptStruct` pointer representing the layout of data entries in the repository.
     */
    UFUNCTION(BlueprintPure, Category = "GameDataRepository")
    const UScriptStruct *GetEntryStruct() const
    {
        return StructProperty->Struct;
    }

    /**
     * @brief Retrieves the number of entries currently stored in the game data repository.
     *
     * This function returns the total count of data entries within the repository.
     * It is useful for determining the size of the data set managed by the repository.
     *
     * @return The total number of entries stored in the repository as an integer.
     */
    UFUNCTION(BlueprintPure, Category = "GameDataRepository")
    int32 GetNumEntries() const
    {
        return GameDataEntries->Num();
    }

    /**
     * @brief Retrieves a game data entry by its unique identifier.
     *
     * This method fetches a pointer to a data entry corresponding to the provided
     * unique identifier. If the entry is not found, the function returns a null pointer.
     *
     * @param ID The unique identifier of the desired game data entry.
     * @return A pointer to the entry's data if found, or nullptr if the entry does not exist.
     */
    const uint8 *GetEntryByID(const FName ID) const
    {
        auto Index = GetRowIndex(ID);
        return Index.IsSet() ? GameDataEntries->GetElementPtr(Index.GetValue()) : nullptr;
    }

    /**
     * @brief Retrieves a pointer to the game data entry at a specified index.
     *
     * This method checks if the provided index is valid within the internal game data
     * entries and returns a pointer to the entry if valid. If the index is invalid,
     * it returns a null pointer.
     *
     * @param Index The index of the desired game data entry.
     * @return A pointer to the entry at the specified index, or nullptr if the index is invalid.
     */
    const uint8 *GetEntryAtIndex(const int32 Index) const
    {
        return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
    }

#if WITH_EDITOR
    /**
     * @brief Retrieves a mutable pointer to the entry at the specified index.
     *
     * This method provides access to a mutable pointer to an entry in the game data
     * repository if the specified index is valid. If the index is invalid, it returns null.
     *
     * @param Index The index of the entry to retrieve.
     * @return A mutable pointer to the entry if the index is valid; otherwise, nullptr.
     */
    uint8 *GetMutableEntryAtIndex(const int32 Index) const
    {
        return GameDataEntries->IsValidIndex(Index) ? GameDataEntries->GetElementPtr(Index) : nullptr;
    }

    /**
     * @brief Checks whether the repository is currently valid.
     *
     * This function evaluates the internal state of the repository to determine if
     * it is considered valid for operations.
     *
     * @return True if the repository is valid, false otherwise.
     */
    bool IsValidRepository() const
    {
        return bValidRepository;
    }
#endif

  protected:
    void PostInitProperties() override;

    void PostLoad() override;

    /**
     * @brief Adds a new entry to the game data repository with the specified ID.
     *
     * This method creates a new entry in the repository, assigns it the provided ID,
     * and manages indexing for efficient lookup. The newly created entry is initialized
     * and stored within the repository's internal data structure.
     *
     * @param ID The unique identifier for the new entry being added.
     * @return A pointer to the newly created entry.
     */
    uint8 *AddNewEntry(FName ID);

    /**
     * @brief Adds a new entry to the game data repository.
     *
     * This method creates and initializes a new entry using the provided ID and data buffer.
     * It ensures the proper structure and properties are set for the new data entry.
     *
     * @param ID The unique identifier for the new entry.
     * @param Data A pointer to the data buffer containing the information to populate the entry.
     * @return A pointer to the newly created entry, or nullptr if the entry could not be added.
     */
    uint8 *AddNewEntry(FName ID, const uint8 *Data);

  private:
    uint8 *AddNewEntryInternal(FName ID);

  protected:
    /**
     * @brief Removes a game data entry at the specified index.
     *
     * This function deletes an entry from the repository's internal data structure
     * based on the given index. It ensures the index is valid and updates related
     * metadata, such as row indices and ID mappings, to maintain consistency within
     * the data repository.
     *
     * @param Index The zero-based index of the entry to be removed.
     *
     * @warning Invoking this function with an invalid index will result in no operation being performed.
     * It is the caller's responsibility to ensure the index is valid.
     *
     * @note The indices of subsequent entries are updated to maintain sequential order after removal.
     * Associated ID-to-index mappings are also adjusted accordingly.
     */
    void RemoveEntryAtIndex(int32 Index);

    /**
     * @brief Swaps the entries stored at the specified indices in the game data repository.
     *
     * This method exchanges the positions of two entries within the internal data structure,
     * ensuring that associated metadata such as indices and IDs are updated accordingly.
     *
     * @param Index1 The index of the first entry to be swapped.
     * @param Index2 The index of the second entry to be swapped.
     * @note Both indices must be valid within the bounds of the repository's data entries.
     */
    void SwapEntries(int32 Index1, int32 Index2);

    /**
     * @brief Reconstructs and updates the indexing system for game data entries.
     *
     * This method clears existing indices and iterates through all game data entries
     * to establish updated index mappings. It ensures proper alignment between data
     * entries and their corresponding indices, enabling efficient lookups by ID.
     *
     * @note This method is typically called to reinitialize or correct index mappings
     * after modifications to the underlying game data entries.
     */
    void RebuildIndices();

    /**
     * @brief Retrieves the ID property of the game data repository entries.
     *
     * This function returns the property object that identifies the unique
     * identifier field for the entries managed by the repository.
     *
     * @return A pointer to the FNameProperty that represents the ID field
     *         of the data entries.
     *
     * @note The returned property is used for operations like lookups and
     *       sorting based on entry IDs.
     */
    FNameProperty *GetIDProperty() const
    {
        return IDProperty;
    }

    /**
     * @brief Retrieves the row index associated with a specific ID.
     *
     * This method searches for the specified ID within the repository and returns
     * the corresponding row index if it exists.
     *
     * @param ID The unique identifier for the data entry to locate.
     * @return An optional integer representing the row index if the ID is found;
     *         otherwise, an empty optional.
     */
    TOptional<int32> GetRowIndex(FName ID) const;

    /**
     * @brief Verifies whether a given row name is unique within the repository.
     *
     * This method checks if the specified name is unique among the entries in the repository.
     * It iterates through all data entries and determines uniqueness based on the provided name.
     *
     * @param Name The name to be verified for uniqueness.
     * @param bVerifyInPlace When true it is assumed that an element has been renamed and we need to find two entries
     *                       with said name in order to be a failure.
     * @return true if the row name is unique; false otherwise.
     */
    bool VerifyRowNameUnique(FName Name, bool bVerifyInPlace = true) const;

  private:
    TOptional<FName> GenerateUniqueRowName() const;

#if WITH_EDITOR
    friend class FGameDataRepositoryEditor;
    friend class UGameDataEntrySerializer;
#endif
    friend class UGameDataRepositoryExporter;

    const FScriptArray *DataArray = nullptr;
    TUniquePtr<FScriptArrayHelper> GameDataEntries;
    FArrayProperty *GameDataEntriesProperty = nullptr;
    FStructProperty *StructProperty = nullptr;
    FNameProperty *IDProperty = nullptr;
    FIntProperty *RowIndexProperty = nullptr;
    TMap<FName, int32> RowIndices;

#if WITH_EDITOR
    bool bValidRepository = false;
#endif
};

/**
 * @class UAssetGameDataRepository
 * @brief Abstract class for managing game data stored in asset files.
 *
 * This class extends the functionality of UGameDataRepository to handle game data
 * that is stored in external asset files. It provides mechanisms for retrieving,
 * indexing, and managing data that exists outside of the primary application data
 * structures, leveraging assets as a source.
 *
 * Designed for scenarios where game data needs to be loaded from or referenced by
 * standalone files, this class supports efficient integration with asset management
 * workflows and tools within the Unreal Engine environment.
 *
 * @note This class is abstract and requires derived concrete implementations for
 * specific asset-related behaviors.
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UAssetGameDataRepository : public UGameDataRepository
{
    GENERATED_BODY()
};

/**
 * @class UStaticGameDataRepository
 * @brief Specialized repository for managing static game data entries.
 *
 * This class extends UGameDataRepository to handle scenarios where the game data
 * being managed is static and not expected to change frequently during runtime.
 * It is tailored for systems that require predefined or immutable data management.
 *
 * The repository provides methods for registering and unregistering data entries,
 * as well as internal mechanisms for validation and error reporting.
 *
 * @note This class is abstract and intended to be used as a base for further
 * specialization in managing static data.
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UStaticGameDataRepository : public UGameDataRepository
{
    GENERATED_BODY()

  protected:
    /**
     * @brief Attempts to register a new entry in the static game data repository.
     *
     * This method is intended to handle the registration logic for a data entry
     * within the repository. It validates the data and updates the repository state
     * accordingly. The specific behavior and implementation details may vary
     * depending on the derived class, but this base implementation should not be
     * called directly.
     *
     * @param OutError A reference to a string that will contain any error message if the registration fails.
     * @param DataStruct A reference to the data structure used for the entry to be registered.
     * @return Returns true if the registration is successful; false otherwise.
     */
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = DataStruct))
    bool TryRegisterEntryInternal(FString &OutError, const int32 &DataStruct);
    DECLARE_FUNCTION(execTryRegisterEntryInternal);

  public:
    /**
     * @brief Attempts to unregister an entry identified by its unique ID from the repository.
     *
     * This method removes the entry associated with the given ID from the repository
     * if it exists. It first checks for the presence of the ID within the repository,
     * and if found, removes the corresponding entry. If the ID is not found, the
     * operation will fail and return false.
     *
     * @param ID The unique identifier of the entry to be unregistered.
     * @return Returns true if the entry was successfully unregistered; false if the ID was not found.
     */
    UFUNCTION(meta = (ScriptMethod))
    bool TryUnregisterEntry(FName ID);
};
