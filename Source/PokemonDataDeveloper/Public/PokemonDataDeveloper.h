#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module that holds the nodes for the Pokémon Data module
 */
class FPokemonDataDeveloperModule : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
