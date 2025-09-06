// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPokemonUI, Log, All)

/**
 * Module for handling the actual menu screens
 */
class FPokemonUIModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
