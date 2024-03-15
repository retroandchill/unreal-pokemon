// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module to handle the Core Pokémon functionality
 */
class FPokemonCoreModule final : public IModuleInterface {
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
