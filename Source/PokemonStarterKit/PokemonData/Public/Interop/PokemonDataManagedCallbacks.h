// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "expected.hpp"
#include "StructUtils/StructView.h"

/**
 * @brief Structure defining managed actions for Pokemon data management.
 *
 * FPokemonDataManagedActions provides a set of function pointers that define
 * specific actions to be performed within the Pokemon data management framework.
 * These actions allow integration with an external system through callbacks.
 */
struct FPokemonDataManagedActions
{
    using FStartDataManager = void(__stdcall *)();
    using FGetEvolutionConditionClass = bool(__stdcall *)(const uint8 *, UScriptStruct **, FString *Error);

    /**
     * @brief Function pointer used to initialize or start the data manager.
     *
     * StartDataManager represents a callback function to initialize or start the
     * Pokemon data management system. This is typically assigned with a function
     * to be invoked externally when the data manager needs to be created or initialized.
     * The function signature follows the `__stdcall` calling convention and does not
     * return any value.
     *
     * It is expected that this function pointer is set to a valid function before
     * invoking any functionality that relies on the data management system being
     * operational.
     */
    FStartDataManager StartDataManager = nullptr;
    /**
     * @brief Function pointer to retrieve the evolution condition class for a specific handle.
     *
     * GetEvolutionConditionClass is a callback function used within the Pokemon data management
     * framework to obtain the appropriate evolution condition class for a given data handle.
     * The function returns a boolean value indicating success or failure, and in case of success,
     * it provides the corresponding UScriptStruct object for the evolution condition class.
     * If an error occurs, an error message is returned.
     *
     * It is expected that this function pointer is implemented and assigned within the system
     * before being called in the workflow.
     */
    FGetEvolutionConditionClass GetEvolutionConditionClass = nullptr;
};

/**
 * @brief Class managing callbacks for Pokemon data operations.
 *
 * FPokemonDataManagedCallbacks serves as a singleton class to manage and interact
 * with externally defined callbacks for Pokemon data operations. It provides functionality
 * to register custom callback actions, control the data manager's lifecycle, and access
 * evolution condition class data through provided handles.
 */
class POKEMONDATA_API FPokemonDataManagedCallbacks
{
    FPokemonDataManagedCallbacks() = default;
    ~FPokemonDataManagedCallbacks() = default;

  public:
    /**
     * @brief Retrieves the singleton instance of FPokemonDataManagedCallbacks.
     *
     * This method provides access to the singleton instance of FPokemonDataManagedCallbacks, enabling
     * centralized management and invocation of callbacks related to Pokemon data management. The instance
     * is lazily initialized upon first access.
     *
     * @return Reference to the singleton instance of FPokemonDataManagedCallbacks.
     */
    static FPokemonDataManagedCallbacks &Get();

    /**
     * @brief Sets the managed callbacks for Pokemon data management actions.
     *
     * This method assigns a set of function pointers to the managed callbacks,
     * defining the actions to be invoked within the Pokemon data management framework.
     * The provided callbacks must be appropriately assigned before invoking any dependent functionality.
     *
     * @param InCallbacks Struct containing function pointers defining the actions
     * to be performed for managing Pokemon data.
     */
    void SetCallbacks(const FPokemonDataManagedActions &InCallbacks);

    /**
     * @brief Initializes the Pokemon data manager using the configured callbacks.
     *
     * Creates and initializes the data manager by invoking the StartDataManager callback,
     * if it has been appropriately assigned. This method integrates the managed data
     * handling by enabling interactions defined in the associated callbacks.
     *
     * It should be ensured that the StartDataManager callback is properly set prior
     * to calling this method. This method is essential for initializing the management
     * processes for Pokemon data within the framework.
     */
    void CreateDataManager() const;

    /**
     * @brief Retrieves the evolution condition class for a given handle.
     *
     * This method retrieves the UScriptStruct representing the evolution condition
     * class associated with the provided handle. It interacts with a callback to
     * obtain the result or an error message if the retrieval fails.
     *
     * @param Handle A pointer to the handle used to identify the evolution condition.
     * @return A tl::expected object containing either a pointer to the UScriptStruct
     *         of the evolution condition class on success, or an error message as
     *         FString if the operation fails.
     */
    tl::expected<UScriptStruct *, FString> GetEvolutionConditionClass(const uint8 *Handle) const;

  private:
    FPokemonDataManagedActions Callbacks;
};
