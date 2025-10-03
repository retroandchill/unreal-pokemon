// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "DataHandle.h"
#include "UObject/Object.h"

#include "GameDataTypeCallbacksExporter.generated.h"

struct FGameDataTypeManagedActions;

/**
 * @class UGameDataTypeCallbacksExporter
 *
 * @brief Provides static utilities for managing and exporting callbacks related to
 * game data types and handle entries.
 *
 * This class offers a series of static functions that assist in the management and
 * exportation of game data type callbacks. It provides functionalities to set managed
 * actions or to add information to data handle entries for further processing.
 *
 * The class is designed to be a utility within the Unreal Engine framework, and it
 * primarily interacts with data structures and callbacks for effective data management.
 *
 * UGameDataTypeCallbacksExporter is part of GAMEDATAACCESSTOOLSEDITOR API.
 *
 * This class is intended for editor and tooling purposes, specifically facilitating
 * the interaction of Unreal Engine with managed code environments.
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataTypeCallbacksExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Sets the managed actions for game data type callbacks.
     *
     * This static method allows users to define or update the managed behavior
     * for game data type callbacks by providing a reference to the managed
     * actions. These actions encapsulate the necessary functions and logic
     * required to handle various aspects of data operations.
     *
     * @param Actions A constant reference to an FGameDataTypeManagedActions struct containing
     *                the definitions of managed actions for game data type callbacks.
     */
    UNREALSHARP_FUNCTION()
    static void SetActions(const FGameDataTypeManagedActions &Actions);

    /**
     * @brief Adds action information to the provided array of data handle entries.
     *
     * This static method appends a new data handle entry to the specified array by
     * transferring ownership of the provided entry. It allows for efficient management
     * and organization of data handle entries when working with game data types.
     *
     * @param DataHandles A reference to the array of FDataHandleEntry where the new entry
     *                    will be added.
     * @param Entry A pointer to the FDataHandleEntry to be moved into the data handle array.
     */
    UNREALSHARP_FUNCTION()
    static void AddActionInformation(TArray<FDataHandleEntry> &DataHandles, FDataHandleEntry *Entry);
};
