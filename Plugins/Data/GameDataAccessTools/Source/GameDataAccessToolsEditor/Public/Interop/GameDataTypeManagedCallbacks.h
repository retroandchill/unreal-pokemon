// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSManagedGCHandle.h"
#include "DataHandle.h"

/**
 * @struct FGameDataTypeManagedActions
 *
 * @brief A structure that defines managed actions used for handling game data type operations.
 *
 * This struct serves as an intermediary for managing game data actions through function pointers.
 * It provides a mechanism to validate game data handles and retrieve options associated with specific
 * game data handles. These callbacks enable integration and communication with managed systems
 * such as C# environments.
 */
struct FGameDataTypeManagedActions
{
    using FIsValidGameDataHandle = bool(__stdcall *)(FGCHandleIntPtr);
    using FGetDataHandleOptions = void(__stdcall *)(FGCHandleIntPtr, TArray<FDataHandleEntry> *);

    /**
     * @typedef FIsValidGameDataHandle
     *
     * @brief A function pointer type used to validate game data handles.
     *
     * This typedef defines a function pointer that accepts an `FGCHandleIntPtr` as input
     * and returns a boolean value indicating whether the provided handle is valid.
     *
     * @param FGCHandleIntPtr A struct representing a managed pointer to the game data handle in C#.
     * @return `true` if the handle is valid, `false` otherwise.
     */
    FIsValidGameDataHandle IsValidGameDataHandle = nullptr;

    /**
     * @typedef FGetDataHandleOptions
     *
     * @brief A function pointer type used to retrieve data handle options associated with a specific game data handle.
     *
     * This typedef defines a function pointer that accepts an `FGCHandleIntPtr` as input and outputs
     * a list of `FDataHandleEntry` objects. It provides a callback mechanism to interact with and retrieve
     * options connected to the game data handle, enabling integration with managed systems such as C# environments.
     *
     * @param FGCHandleIntPtr A struct representing a managed pointer to the game data handle in C#.
     * @param TArray<FDataHandleEntry>* A pointer to an array where the retrieved data handle entries will be stored.
     */
    FGetDataHandleOptions GetDataHandleOptions = nullptr;
};

/**
 * @class FGameDataTypeManagedCallbacks
 *
 * @brief A singleton class that manages callbacks for game data operations.
 *
 * This class provides functionality to manage and interact with callbacks related to game data types.
 * It allows the setting of managed actions, validation of game data handles, and retrieval of data handle options.
 * This facilitates integration with managed systems, such as C# environments, for handling structured data types.
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataTypeManagedCallbacks
{
    FGameDataTypeManagedCallbacks() = default;
    ~FGameDataTypeManagedCallbacks() = default;

  public:
    /**
     * @brief Retrieves the associated value or data from the underlying structure or collection.
     *
     * This function is typically used to access stored information or elements without modifying
     * the original source. The exact behavior and nature of the returned value depend on the
     * specific implementation or context.
     *
     * @return The value or data associated with the given retrieval operation.
     */
    static FGameDataTypeManagedCallbacks &Get();

    /**
     * @brief Sets the actions used to manage game data type operations.
     *
     * This method assigns the provided callback functions encapsulated in `FGameDataTypeManagedActions`
     * to the internal `Actions` member. The callbacks enable interaction with game data handles,
     * including validation and retrieval of associated options. This facilitates integration
     * with managed systems such as C#.
     *
     * @param Callbacks A reference to the `FGameDataTypeManagedActions` structure containing
     *                  the callback functions to be set for handling game data operations.
     */
    void SetActions(const FGameDataTypeManagedActions &Callbacks)
    {
        Actions = Callbacks;
    }

    /**
     * @brief Determines whether the provided game data handle is valid.
     *
     * This method validates the given game data handle by delegating the check to the underlying
     * actions defined in `FGameDataTypeManagedActions`. It ensures that the handle points to
     * a valid managed resource before proceeding with any operations.
     *
     * @param Handle A constant reference to an `FGCHandle` object representing the game data handle
     *               to be validated.
     *
     * @return true if the provided game data handle is considered valid; otherwise, false.
     */
    bool IsValidGameDataHandle(const FGCHandle &Handle) const
    {
        return Actions.IsValidGameDataHandle(Handle.GetHandle());
    }

    /**
     * @brief Retrieves data handle options associated with a given handle.
     *
     * This method queries and retrieves a collection of data handle options
     * related to a specific game data handle. Utilizing the callbacks provided
     * by `Actions`, it interacts with the underlying logic to fetch the relevant
     * entries.
     *
     * @param Handle A constant reference to an `FGCHandle` object representing
     *               the game data handle for which options need to be retrieved.
     *
     * @return A `TArray<FDataHandleEntry>` containing the options or data handle
     *         entries associated with the specified handle.
     */
    TArray<FDataHandleEntry> GetDataHandleOptions(const FGCHandle &Handle) const;

  private:
    FGameDataTypeManagedActions Actions;
};
