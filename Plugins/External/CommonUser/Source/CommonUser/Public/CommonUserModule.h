// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

class FCommonUserModule : public IModuleInterface {
  public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
