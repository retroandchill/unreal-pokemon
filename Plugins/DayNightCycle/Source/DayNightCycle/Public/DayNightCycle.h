﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDayNightCycleModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
