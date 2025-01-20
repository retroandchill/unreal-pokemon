#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimple2DEditorModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
