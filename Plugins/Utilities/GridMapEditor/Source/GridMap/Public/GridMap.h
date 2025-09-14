// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGridMapModule : public IModuleInterface
{
  public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
