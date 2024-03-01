// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Editor module for GridBased 2D library
 */
class FGridBased2DEditorModule : public IModuleInterface {
public:
    void StartupModule() override;

    /**
     * Called after the engine is fully initialized
     */
    void OnPostEngineInit();
	
    void ShutdownModule() override;
};
