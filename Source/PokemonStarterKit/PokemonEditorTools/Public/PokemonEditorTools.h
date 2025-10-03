#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * FPokemonEditorToolsModule is a module implementation for the Pokemon Editor tools.
 * This class is responsible for managing the startup and shutdown behaviors of the module
 * and registering or unregistering custom property type layouts needed for the editor tools.
 */
class FPokemonEditorToolsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;

  private:
    TSharedPtr<class FEvolutionConditionPropertyIdentifier> EvolutionConditionPropertyIdentifier;
};
