#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGridBased2DTestsModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
