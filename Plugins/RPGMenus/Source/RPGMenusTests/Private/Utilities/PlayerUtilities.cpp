// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PlayerUtilities.h"
#include "GameFramework/PlayerState.h"
#include "Player/RPGLocalPlayer.h"
#include "RPGMenus/Private/Player/RPGPlayerController.h"

std::pair<FPlayerPtr, APawn *> UPlayerUtilities::CreateTestPlayer(UWorld &World, const TSubclassOf<APawn> &PawnClass) {
    auto PlayerController = World.SpawnActor<ARPGPlayerController>();
    auto Pawn = World.SpawnActor<APawn>(PawnClass);
    FPlayerPtr Player(NewObject<URPGLocalPlayer>(GEngine));
    World.GetGameInstance()->AddLocalPlayer(Player.Get(), FPlatformUserId::CreateFromInternalId(0));
    PlayerController->SetPlayer(Player.Get());
    PlayerController->Possess(Pawn);
    PlayerController->PlayerState = World.SpawnActor<APlayerState>();
    PlayerController->PlayerState->SetUniqueId(FUniqueNetIdRepl());
    World.AddController(PlayerController);

    return {std::move(Player), Pawn};
}