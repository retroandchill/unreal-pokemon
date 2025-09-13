// Copyright Epic Games, Inc. All Rights Reserved.

#include "RangeV3.h"
#include "HAL/PlatformProcess.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FRangeV3Module"

class FRangeV3Module : public IModuleInterface
{
  public:
    void StartupModule() override
    {
        // No startup required
    }
    void ShutdownModule() override
    {
        // No shutdown required
    }
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRangeV3Module, RangeV3)
