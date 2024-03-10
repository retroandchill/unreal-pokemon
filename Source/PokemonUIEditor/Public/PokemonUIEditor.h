#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Editor module for the Pokémon UI classes
 */
class FPokemonUIEditorModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
