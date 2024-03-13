// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module that handles the loading of Data for a Pokémon game
 */
class FPokemonDataModule final : public IModuleInterface {
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
