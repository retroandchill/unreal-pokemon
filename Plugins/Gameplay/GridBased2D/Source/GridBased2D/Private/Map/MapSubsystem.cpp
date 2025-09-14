// "Unreal Pokémon" created by Retro & Chill.

#include "Map/MapSubsystem.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"
#include "Kismet/GameplayStatics.h"
#include "Map/TileMapGridBasedMap.h"

DECLARE_DELEGATE(FLoadFinalized)

UMapSubsystem::UMapSubsystem(const FObjectInitializer &)
{
}

void UMapSubsystem::WarpToMap(const TSoftObjectPtr<UWorld> &Map, FName WarpTag)
{
    auto PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();
    if (PlayerCharacter == nullptr)
    {
        UE_LOG(LogBlueprint, Warning, TEXT("There is no valid pawn!"))
        return;
    }

    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter);
    if (MovementComponent == nullptr)
    {
        UE_LOG(LogBlueprint, Warning, TEXT("The pawn class does not implement IGridMovable!"))
        return;
    }
    WarpToMapWithDirection(Map, WarpTag, MovementComponent->GetDirection());
}

void UMapSubsystem::WarpToMapWithDirection(const TSoftObjectPtr<UWorld> &Map, FName WarpTag, EFacingDirection Direction)
{
    WarpDestination.Emplace(WarpTag, Direction);
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, Map);
}

void UMapSubsystem::SetPlayerLocation(const TScriptInterface<IGridMovable> &PlayerCharacter)
{
    if (!WarpDestination.IsSet())
    {
        return;
    }

    auto [WarpTag, Direction] = WarpDestination.GetValue();
    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter.GetObject());
    MovementComponent->FaceDirection(Direction);
    WarpDestination.Reset();
}

void UMapSubsystem::UpdateCharacterMapPosition(const TScriptInterface<IGridMovable> &Movable) const
{
    TArray<AActor *> Maps;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UMapGrid::StaticClass(), Maps);
    TScriptInterface<IMapGrid> OldMap = nullptr;
    TScriptInterface<IMapGrid> NewMap = nullptr;
    for (auto Actor : Maps)
    {
        TScriptInterface<IMapGrid> Map = Actor;
        if (OldMap == nullptr && Map->IsCharacterPartOfMap(Movable))
        {
            OldMap = Map;
        }

        if (auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Movable.GetObject());
            NewMap == nullptr && Map->IsPositionInMap(MovementComponent->GetCurrentPosition()))
        {
            NewMap = Map;
        }

        if (OldMap != nullptr && NewMap != nullptr)
        {
            break;
        }
    }

    if (OldMap == NewMap)
    {
        return;
    }

    if (OldMap != nullptr)
    {
        OldMap->RemoveCharacter(Movable);
    }

    if (NewMap != nullptr)
    {
        NewMap->AddCharacter(Movable);
    }
}

const TOptional<TPair<FName, EFacingDirection>> &UMapSubsystem::GetWarpDestination() const
{
    return WarpDestination;
}
