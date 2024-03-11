// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"

/**
 * Module used to handle 2D top-down perspective navigation upon a grid-based system.
 */
class FGridBased2DModule : public IModuleInterface {
public:
	void StartupModule() override;

	void ShutdownModule() override;
};
