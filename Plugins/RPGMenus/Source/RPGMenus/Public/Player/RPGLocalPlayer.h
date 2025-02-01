// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Functional/Delegates.h"

#include "RPGLocalPlayer.generated.h"

class UPrimaryGameLayout;
/**
 * 
 */
UCLASS()
class RPGMENUS_API URPGLocalPlayer : public ULocalPlayer {
    GENERATED_BODY()

public:
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, URPGLocalPlayer* LocalPlayer, APlayerController* PlayerController);
    RETRO_MULTICAST_DELEGATE_MEMBER(FPlayerControllerSetDelegate, OnPlayerControllerSet)
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, URPGLocalPlayer* LocalPlayer, APlayerState* PlayerState);
    RETRO_MULTICAST_DELEGATE_MEMBER(FPlayerStateSetDelegate, OnPlayerStateSet)
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, URPGLocalPlayer* LocalPlayer, APawn* Pawn);
    RETRO_MULTICAST_DELEGATE_MEMBER(FPlayerPawnSetDelegate, OnPlayerPawnSet)

    FDelegateHandle RegisterOnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate);
    FDelegateHandle RegisterOnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate);
    FDelegateHandle RegisterOnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate);
    
    bool GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const override;

    bool IsPlayerViewEnabled() const { return bIsPlayerViewEnabled; }
    void SetIsPlayerViewEnabled(bool bInIsPlayerViewEnabled) { bIsPlayerViewEnabled = bInIsPlayerViewEnabled; }

    UPrimaryGameLayout* GetRootUILayout() const;

private:
    friend class ARPGPlayerController;
    bool bIsPlayerViewEnabled = true;
};
