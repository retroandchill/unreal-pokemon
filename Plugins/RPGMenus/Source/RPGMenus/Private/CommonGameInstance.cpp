// "Unreal Pokémon" created by Retro & Chill.


#include "CommonGameInstance.h"
#include "CommonUISettings.h"
#include "ICommonUIModule.h"
#include "RPGMenus.h"
#include "RPGUIManagerSubsystem.h"
#include "Player/RPGLocalPlayer.h"

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer *NewPlayer, FPlatformUserId UserId) {
    int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
    if (ReturnVal != INDEX_NONE) {
        if (!PrimaryPlayer.IsValid())
        {
            UE_LOG(LogRPGMenus, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
            PrimaryPlayer = NewPlayer;
        }
		
        GetSubsystem<URPGUIManagerSubsystem>()->NotifyPlayerAdded(Cast<URPGLocalPlayer>(NewPlayer));
    }
	
    return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer *ExistingPlayer) {
    if (PrimaryPlayer == ExistingPlayer)
    {
        PrimaryPlayer.Reset();
        UE_LOG(LogRPGMenus, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
    }
    GetSubsystem<URPGUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<URPGLocalPlayer>(ExistingPlayer));

    return Super::RemoveLocalPlayer(ExistingPlayer);
}