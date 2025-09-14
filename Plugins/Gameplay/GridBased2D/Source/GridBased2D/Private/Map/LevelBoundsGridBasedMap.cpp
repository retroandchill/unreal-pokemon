// "Unreal Pokémon" created by Retro & Chill.

#include "Map/LevelBoundsGridBasedMap.h"
#include "GridBased2D.h"
#include "GridUtils.h"

FIntRect ALevelBoundsGridBasedMap::GetBounds() const
{
#if WITH_EDITOR
    if (LevelBounds == nullptr)
    {
        UE_LOG(LogGridBased2D, Error, TEXT("You need to set a valid level bounds for the map object!"))
        return FIntRect();
    }
#endif

    auto BoundingBox = LevelBounds->GetComponentsBoundingBox();

    double GridSize = UGridUtils::GetGridSize(this);
    auto Center = BoundingBox.GetCenter();
    auto Extent = BoundingBox.GetExtent();
    return FIntRect(static_cast<int32>(FMath::RoundToZero((Center.X - Extent.X + GridSize / 2) / GridSize)),
                    static_cast<int32>(FMath::RoundToZero((Center.Y - Extent.Y + GridSize / 2) / GridSize)),
                    static_cast<int32>(FMath::RoundToZero((Center.X + Extent.X + GridSize / 2) / GridSize)),
                    static_cast<int32>(FMath::RoundToZero((Center.Y + Extent.Y + GridSize / 2) / GridSize)));
}