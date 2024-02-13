#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGridBased2DModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
