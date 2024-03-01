// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Editor module for the RPG Menus module
 */
class FRPGMenusEditorModule : public IModuleInterface {
public:
    void StartupModule() override;

	/**
	 * Called after the engine is fully initialized
	 */
	void OnPostEngineInit();
	
    void ShutdownModule() override;
};
