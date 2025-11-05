// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_STATS_GROUP(TEXT("Simple2D"), STATGROUP_Simple2D, STATCAT_Advanced);

class FSimple2DModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
