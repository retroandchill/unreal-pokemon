// "Unreal Pokémon" created by Retro & Chill.


#include "Tileset/Tileset3D.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/ContainerView.h"

const TArray<FTile3D> & UTileset3D::GetTiles() const {
    return Tiles;
}

TArray<FName> UTileset3D::GetTileNames() const {
    return Tiles |
        UE::Ranges::Map(&FTile3D::TileName) |
        UE::Ranges::ToArray;
}