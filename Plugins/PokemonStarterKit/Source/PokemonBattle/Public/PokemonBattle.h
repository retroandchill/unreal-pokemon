#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module that handles all battle related code
 */
class FPokemonBattleModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
