#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * The module that underpins all the basic settings in the starter kit.
 */
class FPokemonSettingsModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
