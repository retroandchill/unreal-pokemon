// "Unreal Pokémon" created by Retro & Chill.


#include "GridMapEditorUtilities.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/IfPresent.h"
#include "Ranges/Views/Map.h"

struct FTileBasicInfo {
    const uint32 TileBitset;
    const double Rotation;
    const int32 Index;
};

constexpr std::array TileBasicInfos = {
    FTileBasicInfo{12, -90, 0},
    FTileBasicInfo{10, 0, 0},
    FTileBasicInfo{5, 180, 0},
    FTileBasicInfo{3, 90, 0},
    FTileBasicInfo{6, 90, 1},
    FTileBasicInfo{9, 0, 1},
    FTileBasicInfo{14, -90, 2},
    FTileBasicInfo{7, 90, 2},
    FTileBasicInfo{13, 180, 2},
    FTileBasicInfo{11, 0, 2},
    FTileBasicInfo{2, 90, 3},
    FTileBasicInfo{8, 0, 3},
    FTileBasicInfo{1, 180, 3},
    FTileBasicInfo{4, -90, 3},
    FTileBasicInfo{140, -90, 4},
    FTileBasicInfo{74, 0, 4},
    FTileBasicInfo{37, 180, 4},
    FTileBasicInfo{19, 90, 4},
    FTileBasicInfo{127, 0, 5},
    FTileBasicInfo{191, 90, 5},
    FTileBasicInfo{239, 180, 5},
    FTileBasicInfo{223, -90, 5},
    FTileBasicInfo{63, 90, 6},
    FTileBasicInfo{207, -90, 6},
    FTileBasicInfo{95, 0, 6},
    FTileBasicInfo{175, 180, 6},
    FTileBasicInfo{173, 180, 7},
    FTileBasicInfo{91, 0, 7},
    FTileBasicInfo{206, -90, 7},
    FTileBasicInfo{55, 90, 7},
    FTileBasicInfo{255, 0, 8},
    FTileBasicInfo{15, 0, 9},
    FTileBasicInfo{0, 0, 10},
    FTileBasicInfo{143, -90, 11},
    FTileBasicInfo{79, 90, 11},
    FTileBasicInfo{47, -90, 11},
    FTileBasicInfo{31, 90, 11},
    FTileBasicInfo{141, 180, 12},
    FTileBasicInfo{75, 0, 13},
    FTileBasicInfo{45, 180, 13},
    FTileBasicInfo{27, 0, 12},
    FTileBasicInfo{142, -90, 13},
    FTileBasicInfo{78, -90, 12},
    FTileBasicInfo{39, 90, 12},
    FTileBasicInfo{23, 90, 13},
    FTileBasicInfo{159, 90, 14},
    FTileBasicInfo{111, 0, 14},
};

static FGridMapTileList MakeTileList(const FTileBasicInfo &Info, const TArray<UStaticMesh *> &Tiles) {
    return {
        .TileAdjacency = {Info.TileBitset},
        .Rotation = FRotator(0, Info.Rotation, 0),
        .Tiles = {Tiles.IsValidIndex(Info.Index) ? Tiles[Info.Index] : nullptr}
    };
}

static void SetTileInfos(UGridMapTileSet &TileSet, const TArray<UStaticMesh *> &Tiles) {
    TileSet.Tiles = TileBasicInfos |
                    UE::Ranges::Map(&MakeTileList, Tiles) |
                    UE::Ranges::ToArray;
}

void UGridMapEditorUtilities::CreateAutoTileLayout(UGridMapTileSet *TileSet, const TArray<UStaticMesh *> &Tiles) {
    UE::Optionals::OfNullable(TileSet) |
        UE::Optionals::IfPresent(&SetTileInfos, Tiles);
}