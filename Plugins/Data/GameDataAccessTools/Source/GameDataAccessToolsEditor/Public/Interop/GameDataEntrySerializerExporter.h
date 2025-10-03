// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "GameDataEntrySerializerExporter.generated.h"

class UAssetGameDataRepository;

/**
 * @class UGameDataEntrySerializerExporter
 *
 * @brief A utility class designed to manage serialization and exporting of game data entries.
 *
 * This class provides functionality to add data to the staging array by utilizing
 * a repository and entry data. It is part of the tools provided within the `GameDataAccessToolsEditor`
 * module to handle game data serialization efficiently.
 *
 * This class is not intended to be instantiated directly as it only contains static functionality.
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntrySerializerExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Adds a data entry to the staging array by utilizing the provided repository and data.
     *
     * This static method is used to manage and serialize game data entries into a staging array,
     * interacting with a game data repository for structure and data handling.
     *
     * @param StagingArray The array where the new entry will be added.
     * @param Repository Pointer to the game data repository containing the entry structure information.
     * @param Data Pointer to the raw data to be copied into the staging array.
     */
    UNREALSHARP_FUNCTION()
    static void AddToStagingArray(FScriptArray &StagingArray, UAssetGameDataRepository *Repository, const uint8 *Data);
};
