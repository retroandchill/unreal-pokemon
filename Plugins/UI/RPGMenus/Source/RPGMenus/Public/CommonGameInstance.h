// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "CommonGameInstance.generated.h"

/**
 *
 */
UCLASS()
class RPGMENUS_API UCommonGameInstance : public UGameInstance {
    GENERATED_BODY()

  public:
    int32 AddLocalPlayer(ULocalPlayer *NewPlayer, FPlatformUserId UserId) override;
    bool RemoveLocalPlayer(ULocalPlayer *ExistingPlayer) override;

  private:
    /** This is the primary player*/
    TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};
