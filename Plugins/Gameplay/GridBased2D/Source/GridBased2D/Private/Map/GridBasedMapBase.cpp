// "Unreal Pokémon" created by Retro & Chill.

#include "Map/GridBasedMapBase.h"
#include "Audio/AudioPlaybackUtils.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"
#include "Kismet/GameplayStatics.h"

void AGridBasedMapBase::BeginPlay()
{
    Super::BeginPlay();

    if (auto Player = UGameplayStatics::GetPlayerPawn(this, 0);
        Player != nullptr && Player->GetClass()->ImplementsInterface(UGridMovable::StaticClass()) &&
        IsObjectInMap(Player))
    {
        OnPlayerEnter();
    }

    TArray<AActor *> InitialCharacters;
    UGameplayStatics::GetAllActorsWithInterface(GetGameInstance(), UGridMovable::StaticClass(), InitialCharacters);
    Characters.Empty();
    for (auto Char : InitialCharacters)
    {
        Characters.Emplace(Char);
    }
}

FIntRect AGridBasedMapBase::GetBounds() const
{
    // Only here because abstract classes aren't allowed in Unreal
    return FIntRect();
}

bool AGridBasedMapBase::IsObjectInMap(TScriptInterface<IGridMovable> Object) const
{
    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Object.GetObject());
    check(MovementComponent != nullptr)
    return IsPositionInMap(MovementComponent->GetCurrentPosition());
}

bool AGridBasedMapBase::IsPositionInMap(const FIntVector2 &Position) const
{
    return GetBounds().Contains({Position.X, Position.Y});
}

bool AGridBasedMapBase::IsCharacterPartOfMap(const TScriptInterface<IGridMovable> &Character) const
{
    return Characters.Contains(Character);
}

void AGridBasedMapBase::AddCharacter(const TScriptInterface<IGridMovable> &Character)
{
    Characters.Emplace(Character);
    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Character.GetObject());
    MovementComponent->OnMapChanged(*this);
}

void AGridBasedMapBase::RemoveCharacter(const TScriptInterface<IGridMovable> &Character)
{
    Characters.Remove(Character);
}

void AGridBasedMapBase::OnPlayerEnter()
{
    UAudioPlaybackUtils::PlayBackgroundMusic(this, BackgroundMusic);
    SetCurrentMapName(DisplayName);
    OnPlayerEnteredMap();
}
