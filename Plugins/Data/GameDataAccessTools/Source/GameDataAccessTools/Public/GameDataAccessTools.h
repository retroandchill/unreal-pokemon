// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * @class FGameDataAccessToolsModule
 * @brief Represents the module implementation for Game Data Access Tools.
 *
 * This class is responsible for managing the lifecycle of the Game Data Access Tools module.
 * It inherits from the IModuleInterface and provides startup and shutdown functionality
 * to initialize and clean up the module, respectively.
 */
class FGameDataAccessToolsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
