#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPokemonItemsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
