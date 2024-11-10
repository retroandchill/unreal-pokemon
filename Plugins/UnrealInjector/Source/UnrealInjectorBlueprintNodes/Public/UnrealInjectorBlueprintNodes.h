#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealInjectorBlueprintNodesModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
