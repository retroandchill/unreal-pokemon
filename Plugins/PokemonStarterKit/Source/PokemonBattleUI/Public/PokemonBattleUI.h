#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for handling the UI of the battle system.
 */
class FPokemonBattleUIModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
