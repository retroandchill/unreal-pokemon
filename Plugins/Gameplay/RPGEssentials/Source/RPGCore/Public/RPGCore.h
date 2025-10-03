// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * @class FRPGCoreModule
 * @brief Represents the core module for the RPG system.
 *
 * This class is responsible for managing the initialization and shutdown
 * processes of the RPG Core module. It implements the IModuleInterface,
 * which requires the implementation of the StartupModule and ShutdownModule
 * methods.
 */
class FRPGCoreModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
