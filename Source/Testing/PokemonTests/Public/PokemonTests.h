#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for handling the new tests.
 */
class FPokemonTestsModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;

  private:
    void PostEngineInit();
};
