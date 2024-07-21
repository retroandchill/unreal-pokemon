// "Unreal Pokémon" created by Retro & Chill.

#include "GridBased2DSettings.h"

double UGridBased2DSettings::GetGridSize() const {
    return GridSize;
}

const FSoftObjectPath &UGridBased2DSettings::GetTerrainTagDataTable() const {
    return TerrainTagDataTable;
}
