// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * @class FCommonUtilitiesModule
 * @brief Implements the module interface for the Common Utilities module.
 *
 * The FCommonUtilitiesModule class is responsible for handling the lifecycle of the Common Utilities module,
 * including startup and shutdown behavior. It inherits from the IModuleInterface.
 */
class FCommonUtilitiesModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
