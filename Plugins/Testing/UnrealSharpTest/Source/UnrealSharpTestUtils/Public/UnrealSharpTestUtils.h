#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealSharpTestUtils, Log, All);

class FUnrealSharpTestUtilsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
