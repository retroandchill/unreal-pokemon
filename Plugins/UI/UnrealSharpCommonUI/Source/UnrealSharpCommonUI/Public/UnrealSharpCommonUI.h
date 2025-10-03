// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * @class FUnrealSharpCommonUIModule
 * @brief Implements the module interface for UnrealSharp Common UI.
 *
 * The FUnrealSharpCommonUIModule class provides startup and shutdown
 * functionality for the UnrealSharpCommonUI module. It overrides methods
 * from the IModuleInterface to facilitate module lifecycle management.
 */
class FUnrealSharpCommonUIModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
