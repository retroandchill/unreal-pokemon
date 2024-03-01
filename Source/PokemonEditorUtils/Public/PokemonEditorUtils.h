// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for handling editor utility functions within the engine
 */
class FPokemonEditorUtilsModule final : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
