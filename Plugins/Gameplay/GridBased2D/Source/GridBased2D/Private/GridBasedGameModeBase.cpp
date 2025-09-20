// "Unreal Pokémon" created by Retro & Chill.
#include "GridBasedGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GridBased2DSettings.h"
#include "Map/MapSubsystem.h"

double AGridBasedGameModeBase::GetGridSize() const
{
    static const double DefaultGridSize = GetDefault<UGridBased2DSettings>()->GetGridSize();
    return GridSize.Get(DefaultGridSize);
}

AActor *AGridBasedGameModeBase::ChoosePlayerStart_Implementation(AController *Player)
{
    auto Subsystem = GetGameInstance()->GetSubsystem<UMapSubsystem>();
    check(Subsystem != nullptr)

    auto &WarpDestination = Subsystem->GetWarpDestination();
    if (!WarpDestination.IsSet())
    {
        return Super::ChoosePlayerStart_Implementation(Player);
    }

    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        if (It->PlayerStartTag == WarpDestination->Key)
        {
            return *It;
        }
    }

    return nullptr;
}

void AGridBasedGameModeBase::ScreenTransitionFinished() const
{
    OnScreenTransitionFinished.Broadcast();
}
