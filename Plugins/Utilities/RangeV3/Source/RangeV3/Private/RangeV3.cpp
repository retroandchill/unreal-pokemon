// Copyright Epic Games, Inc. All Rights Reserved.

#include "RangeV3.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

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
