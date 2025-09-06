// "Unreal Pokémon" created by Retro & Chill.
#include "GridUtils.h"
#include "GridBased2DSettings.h"
#include "GridBasedGameModeBase.h"
#include "Kismet/GameplayStatics.h"

double UGridUtils::GetDefaultGridSize()
{
    static const double DefaultGridSize = GetDefault<UGridBased2DSettings>()->GetGridSize();
    return DefaultGridSize;
}

double UGridUtils::GetGridSize(const UObject *WorldContext)
{
    auto GridBasedGameMode = CastChecked<AGridBasedGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    return GridBasedGameMode->GetGridSize();
}

TOptional<EFacingDirection> UGridUtils::VectorToFacingDirection(const FVector2D Vector)
{
    using enum EFacingDirection;
    if (Vector.Y > 0)
    {
        return Down;
    }

    if (Vector.X < 0)
    {
        return Left;
    }

    if (Vector.X > 0)
    {
        return Right;
    }

    if (Vector.Y < 0)
    {
        return Up;
    }

    return TOptional<EFacingDirection>();
}

void UGridUtils::VectorToFacingDirection(const FVector2D &Vector, EFacingDirection &Direction, EValidDirection &Execs)
{
    if (auto ParsedDir = VectorToFacingDirection(Vector); ParsedDir.IsSet())
    {
        Direction = ParsedDir.GetValue();
        Execs = EValidDirection::HasDirection;
    }
    else
    {
        Execs = EValidDirection::NoDirection;
    }
}

EFacingDirection UGridUtils::GetOpposingDirection(EFacingDirection Direction)
{
    switch (Direction)
    {
        using enum EFacingDirection;
    case Down:
        return Up;
    case Left:
        return Right;
    case Right:
        return Left;
    case Up:
        return Down;
    }

    return Direction;
}

AGridBasedGameModeBase *UGridUtils::GetGridBasedGameMode(const UObject *WorldContext)
{
    auto GameMode = Cast<AGridBasedGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    check(GameMode != nullptr)
    return GameMode;
}
