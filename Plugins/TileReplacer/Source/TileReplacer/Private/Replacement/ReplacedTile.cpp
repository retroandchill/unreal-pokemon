// "Unreal Pokémon" created by Retro & Chill.

#include "Replacement/ReplacedTile.h"

FReplacedTile::FReplacedTile() = default;

FReplacedTile::FReplacedTile(const FPaperTileInfo &TileInfo, int32 X, int32 Y, int32 Layer, AActor *Replacement)
    : OriginalTileInfo(TileInfo), TileX(X), TileY(Y), TileLayer(Layer), Replacement(Replacement) {
}
