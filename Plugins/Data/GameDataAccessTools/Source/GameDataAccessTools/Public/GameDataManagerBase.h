// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GameDataManagerBase.generated.h"

class UAssetGameDataRepository;

/**
 * @class UGameDataManagerBase
 * @brief This is an abstract base class responsible for creating and managing game data managers.
 *
 * UGameDataManagerBase serves as a foundation for creating and destroying game data manager instances.
 * It provides lifecycle methods that can be extended by subclasses to handle specific initialization
 * and deinitialization logic through Blueprint events.
 *
 * @note This class should be subclassed to implement specific game data management functionality.
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UGameDataManagerBase : public UObject
{
    GENERATED_BODY()

  protected:
    /**
     * @brief Creates an instance of a specified game data manager class and initializes it.
     *
     * This static method dynamically constructs an instance of the specified game data manager class,
     * assigns it a unique name, and ensures its initialization process completes.
     *
     * @param ManagerClass The class type of the game data manager to instantiate. Must derive from
     * UGameDataManagerBase.
     * @param Name The name to assign to the newly created game data manager instance.
     * @return A pointer to the newly created and initialized instance of UGameDataManagerBase.
     */
    UFUNCTION(meta = (ScriptMethod, DeterminesOutputType = "ManagerClass", DynamicOutputParam = "ReturnValue"))
    static UGameDataManagerBase *Create(TSubclassOf<UGameDataManagerBase> ManagerClass, FName Name);

    /**
     * @brief Releases resources and performs necessary cleanup for the game data manager instance.
     *
     * The Destroy method is responsible for deinitializing the game data manager instance
     * and resetting its state. It invokes the Deinitialize method to allow for custom cleanup logic
     * and clears the object's root set flag to enable proper garbage collection.
     *
     * @note Subclasses can implement additional cleanup behavior by overriding the Deinitialize method.
     */
    UFUNCTION(meta = (ScriptMethod))
    void Destroy();

    /**
     * @brief Blueprint implementable event for initializing the game data manager instance.
     *
     * The Initialize method is intended to be overridden in Blueprints to perform
     * custom initialization logic specific to the game data manager instance. It is
     * automatically called after the creation of the manager instance through the Create
     * method to ensure proper setup.
     *
     * @note This method has no default implementation in C++ and should be implemented in Blueprints/C#.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataManager")
    void Initialize();

    /**
     * @brief Blueprint implementable event for deinitializing the game data manager instance.
     *
     * The Deinitialize method is intended to be overridden in Blueprints to perform
     * custom cleanup and resource release specific to the game data manager instance.
     * It is invoked automatically during the destruction process of the manager instance,
     * ensuring proper deinitialization.
     *
     * @note This method has no default implementation in C++ and should be implemented in Blueprints/C#.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataManager")
    void Deinitialize();
};
