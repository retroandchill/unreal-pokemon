// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module for handling the actual menu screens
 */
class FPokemonUIModule : public IModuleInterface {
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
