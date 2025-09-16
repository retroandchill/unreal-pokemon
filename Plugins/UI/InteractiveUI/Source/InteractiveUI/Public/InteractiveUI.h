// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteractiveUI, Log, All)
DECLARE_STATS_GROUP(TEXT("InteractiveUI"), STATGROUP_InteractiveUI, STATCAT_Advanced);

/**
 * Module for the basic types of Windows used by the UI System
 */
class FInteractiveUIModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
