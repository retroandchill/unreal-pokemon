// "Unreal Pokémon" created by Retro & Chill.
#include "GridBased2D.h"

constexpr auto GLoctextNamespace = "FGridBased2DModule";

void FGridBased2DModule::StartupModule() {
	// No special startup required
}

void FGridBased2DModule::ShutdownModule() {
    // No special shutdown required
}

IMPLEMENT_MODULE(FGridBased2DModule, GridBased2D)
