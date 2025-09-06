// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "RPGPlayerController.generated.h"

/**
 *
 */
UCLASS()
class RPGMENUS_API ARPGPlayerController : public APlayerController
{
    GENERATED_BODY()

  public:
    void ReceivedPlayer() override;
    void SetPawn(APawn *InPawn) override;
    void OnPossess(APawn *APawn) override;
    void OnUnPossess() override;

  protected:
    void OnRep_PlayerState() override;
};
