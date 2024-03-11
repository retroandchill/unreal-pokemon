// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Module of utility functions/classes for shared math operations
 */
class FCommonMathModule : public IModuleInterface {
public:
	void StartupModule() override;
	void ShutdownModule() override;
};
