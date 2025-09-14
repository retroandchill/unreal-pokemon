// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for handling dependency injection for classes
 */
class FUnrealInjectorModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
