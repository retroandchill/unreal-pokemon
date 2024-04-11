// "Unreal Pokémon" created by Retro & Chill.

#include "Replacement/TileReplacement.h"

uint32 GetTypeHash(const FPaperTileInfo &TileInfo) {
    return HashCombine(GetTypeHash(TileInfo.TileSet), GetTypeHash(TileInfo.PackedTileIndex));
}
