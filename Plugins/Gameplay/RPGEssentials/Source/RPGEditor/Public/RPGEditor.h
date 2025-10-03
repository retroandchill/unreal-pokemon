#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * @class FRPGEditorModule
 * @brief Handles the module interface for the role-playing game (RPG) editor module.
 *
 * The FRPGEditorModule class is responsible for the lifecycle management of the RPG editor module in Unreal Engine.
 * It is derived from IModuleInterface and implements the required methods to initialize and shut down the module.
 */
class FRPGEditorModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
