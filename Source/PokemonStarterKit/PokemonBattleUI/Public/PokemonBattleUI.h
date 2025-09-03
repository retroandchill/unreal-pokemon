#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBattleUI, Log, All)

/**
 * Module for handling the UI of the battle system.
 */
class FPokemonBattleUIModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
