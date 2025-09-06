// "Unreal Pokémon" created by Retro & Chill.
#include "GridBasedGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GridBased2DSettings.h"
#include "Map/MapSubsystem.h"
#include "RetroLib/Async/AsyncHelpers.h"

double AGridBasedGameModeBase::GetGridSize() const
{
    static const double DefaultGridSize = GetDefault<UGridBased2DSettings>()->GetGridSize();
    return GridSize.Get(DefaultGridSize);
}

UE5Coro::TCoroutine<> AGridBasedGameModeBase::FadeIn(FForceLatentCoroutine)
{
    co_await Retro::BindToDelegateDispatch(OnScreenTransitionFinished, [this] { ScreenFadeIn(); });
}

UE5Coro::TCoroutine<> AGridBasedGameModeBase::FadeOut(FForceLatentCoroutine)
{
    co_await Retro::BindToDelegateDispatch(OnScreenTransitionFinished, [this] { ScreenFadeOut(); });
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
