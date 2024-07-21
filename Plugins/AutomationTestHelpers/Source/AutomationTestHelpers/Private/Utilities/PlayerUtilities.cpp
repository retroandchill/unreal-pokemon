// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PlayerUtilities.h"
#include "CommonLocalPlayer.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerState.h"

std::pair<FPlayerPtr, APawn *> UPlayerUtilities::CreateTestPlayer(UWorld &World, const TSubclassOf<APawn> &PawnClass) {
    auto PlayerController = World.SpawnActor<ACommonPlayerController>();
    auto Pawn = World.SpawnActor<APawn>(PawnClass);
    FPlayerPtr Player(NewObject<UCommonLocalPlayer>(GEngine));
    World.GetGameInstance()->AddLocalPlayer(Player.Get(), FPlatformUserId::CreateFromInternalId(0));
    PlayerController->SetPlayer(Player.Get());
    PlayerController->Possess(Pawn);
    PlayerController->PlayerState = World.SpawnActor<APlayerState>();
    PlayerController->PlayerState->SetUniqueId(FUniqueNetIdRepl());
    World.AddController(PlayerController);

    return {MoveTemp(Player), Pawn};
}