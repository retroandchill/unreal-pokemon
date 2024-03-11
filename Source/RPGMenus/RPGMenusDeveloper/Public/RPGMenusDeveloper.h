// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Developer modules for the RPG Menus nodes
 */
class FRPGMenusDeveloperModule : public IModuleInterface {
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
