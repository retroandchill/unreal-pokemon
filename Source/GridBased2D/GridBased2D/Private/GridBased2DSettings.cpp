// "Unreal Pokémon" created by Retro & Chill.


#include "GridBased2DSettings.h"

double UGridBased2DSettings::GetGridSize() const {
	return GridSize;
}

FVector UGridBased2DSettings::GetDynamicLevelOffset() const {
	return DynamicLevelOffset;
}
