// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PlayerUtilities.h"

std::pair<FPlayerPtr, APawn *> UPlayerUtilities::CreateTestPlayer(UWorld &World) {
    auto PlayerController = World.SpawnActor<APlayerController>();
    auto Pawn = World.SpawnActor<APawn>();
    FPlayerPtr Player(NewObject<ULocalPlayer>(GEngine));
    PlayerController->Possess(Pawn);
    PlayerController->SetPlayer(Player.Get());
    Player->PlayerAdded(World.GetGameViewport(), 0);

    return {MoveTemp(Player), Pawn};
}