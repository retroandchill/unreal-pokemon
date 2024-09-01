#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealRangesTestModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
