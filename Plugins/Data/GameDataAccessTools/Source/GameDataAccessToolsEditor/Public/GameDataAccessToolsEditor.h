#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Serialization/GameDataEntrySerializer.h"

class FGameDataAccessToolsEditorModule;
class FGameDataRepositoryActions;

/**
 * IGameDataAccessToolsEditorModule is an interface for the Game Data Access Tools Editor module.
 * It provides tools and utilities to interact with serializers for game data repositories in
 * the editor.
 *
 * This interface should be implemented by modules that need to offer platform-specific
 * or project-specific serializers for use in the editor.
 */
class GAMEDATAACCESSTOOLSEDITOR_API IGameDataAccessToolsEditorModule : public IModuleInterface
{
  public:
    /**
     * Retrieves the instance of the IGameDataAccessToolsEditorModule that handles access to game data-related
     * functionalities in the editor. This static method ensures that the appropriate module is loaded and active.
     *
     * @return A reference to the loaded IGameDataAccessToolsEditorModule instance.
     */
    static IGameDataAccessToolsEditorModule &Get();

    /**
     * Retrieves a list of available serializers compatible with the specified repository class.
     * These serializers are used for handling game data entry serialization within the editor.
     *
     * @param RepositoryClass The class type of the game data repository for which serializers are required.
     * @return An array of pointers to the available serializers compatible with the specified repository class.
     */
    virtual TArray<TObjectPtr<const UGameDataEntrySerializer>> GetAvailableSerializers(
        TSubclassOf<UAssetGameDataRepository> RepositoryClass) const = 0;
};
