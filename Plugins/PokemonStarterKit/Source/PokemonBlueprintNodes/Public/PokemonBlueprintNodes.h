#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module that handles the Blueprint Nodes used by the Pokémon Starter Kit
 */
class FPokemonBlueprintNodesModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
