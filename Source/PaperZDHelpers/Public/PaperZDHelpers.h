#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPaperZDHelpersModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
