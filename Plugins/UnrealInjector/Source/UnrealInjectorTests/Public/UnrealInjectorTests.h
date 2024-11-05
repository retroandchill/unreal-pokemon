#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealInjectorTestsModule : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
