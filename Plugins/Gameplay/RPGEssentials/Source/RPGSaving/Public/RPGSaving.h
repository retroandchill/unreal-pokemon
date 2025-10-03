#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * @class FRPGSavingModule
 * @brief Represents the saving module for the RPG system, implementing the IModuleInterface.
 *
 * This module is responsible for handling the initialization and shutdown
 * routines required for integrating saving functionalities within the RPG system.
 */
class FRPGSavingModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
