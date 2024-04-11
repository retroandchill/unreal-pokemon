// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for the developer aspect of the Pokémon UI
 */
class FPokemonUIDeveloperModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
