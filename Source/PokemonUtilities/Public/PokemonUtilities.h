// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Utility module that contains basic functionality for the Unreal Pokémon project
 */
class FPokemonUtilitiesModule final : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
