// "Unreal Pokémon" created by Retro & Chill.

#include "Components/GridBasedMovementComponent.h"
#include "Characters/MoveCheckResult.h"
#include "Components/GridMovable.h"
#include "GridUtils.h"
#include "Interaction/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Map/MapSubsystem.h"
#include "Map/TileMapGridBasedMap.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "RetroLib/Utils/BlueprintMathUtils.h"
#include "RetroLib/Utils/Math.h"

UGridBasedMovementComponent::UGridBasedMovementComponent() : CurrentPosition(0, 0), DesiredPosition(0, 0)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGridBasedMovementComponent::SetPositionInGrid(FVector Position)
{
    auto GridSize = UGridUtils::GetGridSize(this);
    CurrentPosition.X = FMath::FloorToInt32(Position.X / GridSize);
    CurrentPosition.Y = FMath::FloorToInt32(Position.Y / GridSize);
    DesiredPosition = CurrentPosition;
    Position.X = CurrentPosition.X * GridSize;
    Position.Y = CurrentPosition.Y * GridSize;
    GetOwner()->SetActorLocation(Position, bPerformSweep);
}

void UGridBasedMovementComponent::SetMoveTime(double Time)
{
    MoveTime = Time;
}

void UGridBasedMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    auto Owner = GetOwner();
    if (Owner == nullptr)
    {
        return;
    }
    auto Position = Owner->GetActorLocation();
    SetPositionInGrid(Position);

    if (auto Pawn = Cast<APawn>(Owner); Pawn == nullptr || !Pawn->IsPlayerControlled())
    {
        return;
    }
    if (auto MapSubsystem = Owner->GetGameInstance()->GetSubsystem<UMapSubsystem>();
        MapSubsystem != nullptr && Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
    {
        MapSubsystem->SetPlayerLocation(Owner);
    }
}

// Called every frame
void UGridBasedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateMovement(DeltaTime);
    UpdateAnimation(DeltaTime);
}

UE5Coro::TCoroutine<> UGridBasedMovementComponent::MoveInDirection(EFacingDirection MovementDirection,
                                                                   FForceLatentCoroutine)
{
    if (MoveCallback.IsBound())
    {
        UE_LOG(LogBlueprint, Warning, TEXT("The movement timer is already set for character: %s"),
               *GetOwner()->GetName())
        co_return;
    }

    FaceDirection(MovementDirection);
    if (auto [bCanMove, FoundActors] = MovementCheck(MovementDirection); !bCanMove)
    {
        HitInteraction(FoundActors);
        co_return;
    }

    UGridUtils::AdjustMovementPosition(MovementDirection, DesiredPosition);

    MoveTimer.Emplace(0.f);
    StopTimer.Reset();
    bIsMoving = true;
    OnMovementStateChange.Broadcast(true);
    co_await MoveCallback;
}

FMoveCheckResult UGridBasedMovementComponent::MovementCheck(EFacingDirection MovementDirection) const
{
    FMoveCheckResult Ret;
    TArray<AActor *> Maps;
    UGameplayStatics::GetAllActorsWithInterface(this, UMapGrid::StaticClass(), Maps);
    auto DestinationPosition = GetCurrentPosition();
    UGridUtils::AdjustMovementPosition(MovementDirection, DestinationPosition);
    bool bMapFound = false;
    for (auto Actor : Maps)
    {
        if (TScriptInterface<IMapGrid> Map = Actor; Map->IsPositionInMap(DestinationPosition))
        {
            bMapFound = Map->IsObjectInMap(GetOwner()) || CanMoveBetweenMaps();
            break;
        }
    }

    if (!bMapFound)
    {
        Ret.bCanMove = false;
        return Ret;
    }

    UPrimitiveComponent *BlockingComponent = nullptr;
    for (auto Results = HitTestOnFacingTile(MovementDirection); const auto &Result : Results)
    {
        if (Result.bBlockingHit)
        {
            Ret.bCanMove = false;
            BlockingComponent = Result.GetComponent();
        }

        if (auto Interactable = Cast<IInteractable>(Result.GetActor()); Interactable != nullptr)
        {
            Ret.FoundActors.Emplace(Result.GetActor());
        }
    }

    if (auto Owner = GetOwner(); Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
    {
        FVector LocalOffset(0, 0, 0);
        UGridUtils::AdjustMovementPosition(MovementDirection, LocalOffset);
        auto Position = Owner->GetActorLocation();
        auto GridPosition = LocalOffset * UGridUtils::GetGridSize(this) + Position;
        Ret.bCanMove = !IGridMovable::Execute_PerformAdditionalMovementChecks(Owner, GridPosition, !Ret.bCanMove,
                                                                              BlockingComponent);
    }

    return Ret;
}

bool UGridBasedMovementComponent::CanMoveBetweenMaps() const
{
    auto Owner = GetOwner<APawn>();
    return Owner != nullptr && Owner->IsPlayerControlled();
}

void UGridBasedMovementComponent::FaceDirection(EFacingDirection FacingDirection)
{
    Direction = FacingDirection;
    OnDirectionChange.Broadcast(Direction);
}

void UGridBasedMovementComponent::WarpToLocation(int32 X, int32 Y, FVector Offset)
{
    auto GridSize = UGridUtils::GetGridSize(this);
    CurrentPosition =
        DesiredPosition = {FMath::FloorToInt32(Offset.X / GridSize) + X, FMath::FloorToInt32(Offset.Y / GridSize) + Y};
    Offset.X += X * GridSize;
    Offset.Y += Y * GridSize;
    GetOwner()->SetActorLocation(Offset);
}

void UGridBasedMovementComponent::OnMapChanged(IMapGrid &NewMap) const
{
    if (auto Owner = GetOwner<APawn>(); Owner == nullptr || !Owner->IsPlayerControlled())
    {
        return;
    }

    NewMap.OnPlayerEnter();
}

FIntVector2 UGridBasedMovementComponent::GetCurrentPosition() const
{
    return CurrentPosition;
}

FIntVector2 UGridBasedMovementComponent::GetDesiredPosition() const
{
    return DesiredPosition;
}

EFacingDirection UGridBasedMovementComponent::GetDirection() const
{
    return Direction;
}

bool UGridBasedMovementComponent::IsMoving() const
{
    return bIsMoving;
}

void UGridBasedMovementComponent::MoveInput(const FVector2D &InputVector)
{
    auto Dir = UGridUtils::VectorToFacingDirection(InputVector);
    if (!Dir.IsSet() || CurrentPosition != DesiredPosition)
        return;

    MoveInDirection(Dir.GetValue());
}

void UGridBasedMovementComponent::TurnInput(const FVector2D &InputVector)
{
    auto Dir = UGridUtils::VectorToFacingDirection(InputVector);
    if (!Dir.IsSet() || CurrentPosition != DesiredPosition)
        return;

    FaceDirection(Dir.GetValue());
}

TArray<FOverlapResult> UGridBasedMovementComponent::HitTestOnFacingTile(EFacingDirection MovementDirection) const
{
    static const auto FloatGridSize = static_cast<float>(UGridUtils::GetGridSize(this));

    FVector LocalOffset(0, 0, 0);
    UGridUtils::AdjustMovementPosition(MovementDirection, LocalOffset);

    auto Owner = GetOwner();
    auto Position = Owner->GetActorLocation();
    auto GridPosition = LocalOffset * FloatGridSize + Position;

    FCollisionShape GridSquare;
    GridSquare.SetBox(FVector3f(FloatGridSize / 4 - 2, FloatGridSize / 4 - 2, FloatGridSize / 4 - 2));
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    TArray<FOverlapResult> Result;
    GetWorld()->OverlapMultiByChannel(Result, GridPosition, Owner->GetActorRotation().Quaternion(), ECC_Pawn,
                                      GridSquare, Params);

    return Result;
}

TArray<TScriptInterface<IInteractable>> UGridBasedMovementComponent::InteractTestOnFacingTile(
    EFacingDirection MovementDirection) const
{
    auto Results = HitTestOnFacingTile(MovementDirection);
    // clang-format off
    return Results |
           Retro::Ranges::Views::Transform(&FOverlapResult::GetActor) |
           Retro::Ranges::Views::Filter(&AActor::Implements<UInteractable>) |
           Retro::Ranges::Views::Transform(Retro::AsInterface<IInteractable>) |
           Retro::Ranges::To<TArray>();
    // clang-format on
}

void UGridBasedMovementComponent::HitInteraction(const TArray<TScriptInterface<IInteractable>> &Interactables) const
{
    if (auto Owner = GetOwner<APawn>(); Owner == nullptr || !Owner->IsPlayerControlled())
    {
        return;
    }

    for (auto &Interactable : Interactables)
    {
        if ((static_cast<std::byte>(IInteractable::Execute_GetInteractionTypes(Interactable.GetObject())) &
             static_cast<std::byte>(EInteractionType::Hit)) == static_cast<std::byte>(0))
            continue;
        IInteractable::Execute_OnInteract(Interactable.GetObject(), GetOwner(), EInteractionType::Hit);
    }
}

void UGridBasedMovementComponent::UpdateMovement(float DeltaTime)
{
    if (!MoveTimer.IsSet())
        return;

    auto Pos = CurrentPosition;
    float &Timer = MoveTimer.GetValue();
    Timer += DeltaTime;

    auto Owner = GetOwner();
    auto Position = Owner->GetActorLocation();
    auto GridSize = UGridUtils::GetGridSize(this);
    if (CurrentPosition.X != DesiredPosition.X)
    {
        int32 Distance = FMath::Abs(CurrentPosition.X - DesiredPosition.X);
        Position.X = Retro::LinearInterpolation(CurrentPosition.X * GridSize, DesiredPosition.X * GridSize,
                                                MoveTime * Distance, Timer);

        if (Timer >= MoveTime * Distance)
        {
            CurrentPosition.X = DesiredPosition.X;
        }
    }

    if (CurrentPosition.Y != DesiredPosition.Y)
    {
        int32 Distance = FMath::Abs(CurrentPosition.Y - DesiredPosition.Y);
        Position.Y = Retro::LinearInterpolation(CurrentPosition.Y * GridSize, DesiredPosition.Y * GridSize,
                                                MoveTime * Distance, Timer);

        if (Timer >= MoveTime * Distance)
        {
            CurrentPosition.Y = DesiredPosition.Y;
        }
    }

    Owner->SetActorLocation(Position, bPerformSweep);
    if (Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
    {
        IGridMovable::Execute_AdjustCharacterPlacement(Owner);
    }
    if (CurrentPosition != Pos && CurrentPosition == DesiredPosition)
    {
        MoveComplete();
    }
}

void UGridBasedMovementComponent::UpdateAnimation(float DeltaTime)
{
    if (StopTimer.IsSet())
    {
        auto &Timer = StopTimer.GetValue();
        Timer += DeltaTime;

        if (Timer >= 0.125f)
        {
            bIsMoving = false;
            OnMovementStateChange.Broadcast(false);
            StopTimer.Reset();
        }
    }
}

void UGridBasedMovementComponent::MoveComplete()
{
    MoveTimer.Reset();
    StopTimer.Emplace(0.f);

    MoveCallback.ExecuteIfBound();
    OnTakeStep.Broadcast();

    auto Owner = GetOwner();
    auto MapSubsystem = Owner->GetGameInstance()->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)
    check(Owner != nullptr && Owner->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
    MapSubsystem->UpdateCharacterMapPosition(Owner);
}