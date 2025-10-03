// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "GameDataRepositoryExporter.generated.h"

class UGameDataRepository;
/**
 * @class UGameDataRepositoryExporter
 * @brief A utility class used to export and retrieve data from a UGameDataRepository.
 *
 * This class offers static functions to access entries in a UGameDataRepository, either as a whole,
 * by specific ID, or by index.
 *
 * It serves as a data exporter for game data repositories, providing direct access to raw
 * repository data through the `FScriptArray` and `uint8` pointers.
 */
UCLASS()
class GAMEDATAACCESSTOOLS_API UGameDataRepositoryExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Retrieves the entries from the specified UGameDataRepository.
     *
     * This function provides access to the raw data entries within a UGameDataRepository.
     * It returns a reference to the internal data array representing the repository's entries.
     *
     * @param Repository A pointer to the UGameDataRepository object from which entries are retrieved.
     * @return A pointer to the FScriptArray containing the data entries of the specified UGameDataRepository,
     *         or nullptr if the repository is invalid.
     */
    UNREALSHARP_FUNCTION()
    static const FScriptArray *GetEntries(const UGameDataRepository *Repository);

    /**
     * @brief Retrieves a specific entry from the given UGameDataRepository by its unique ID.
     *
     * This function allows access to a single data entry within a UGameDataRepository, identified by
     * the provided ID. The returned pointer corresponds to the raw memory location of the entry within
     * the repository.
     *
     * @param Repository A pointer to the UGameDataRepository object from which the entry is retrieved.
     * @param ID The unique identifier of the entry to retrieve.
     * @return A pointer to the entry's data if found, or nullptr if no entry matches the provided ID
     *         or the repository is invalid.
     */
    UNREALSHARP_FUNCTION()
    static const uint8 *GetEntryByID(const UGameDataRepository *Repository, FName ID);

    /**
     * @brief Retrieves a specific entry from the given UGameDataRepository by its index.
     *
     * This function provides access to an entry within a UGameDataRepository specified
     * by its index. It returns a pointer to the raw memory location of the entry in the repository.
     *
     * @param Repository A pointer to the UGameDataRepository object from which the entry is retrieved.
     * @param Index The index of the entry to retrieve within the repository.
     * @return A pointer to the entry's data if the index is valid, or nullptr if the index is out of range
     *         or the repository is invalid.
     */
    UNREALSHARP_FUNCTION()
    static const uint8 *GetEntryByIndex(const UGameDataRepository *Repository, int32 Index);
};
