// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRPGMenus, Log, All)

/**
 * Module for the basic types of Windows used by the UI System
 */
class FRPGMenusModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
