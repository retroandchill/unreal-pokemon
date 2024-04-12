#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * @class FTileReplacerModule
 * @brief This class is an implementation of the IModuleInterface interface.
 *
 * The FTileReplacerModule class is responsible for managing the startup and shutdown of the tile replacer module.
 * It inherits from the IModuleInterface class, which ensures that it can be properly managed by the engine.
 */
class FTileReplacerModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
