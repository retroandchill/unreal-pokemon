#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBattle, Log, All)

/**
 * Module that handles all battle related code
 */
class FPokemonBattleModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;

  private:
    void OnPostEngineInit();
};
