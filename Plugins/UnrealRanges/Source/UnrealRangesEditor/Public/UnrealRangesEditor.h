#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealRangesEditorModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
