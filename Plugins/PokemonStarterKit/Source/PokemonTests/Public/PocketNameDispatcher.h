// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "PocketNameDispatcher.generated.h"

/**
 * Object that is used to check that the pocket name was dispatched.
 */
UCLASS()
class POKEMONTESTS_API UPocketNameDispatcher : public UObject {
    GENERATED_BODY()

  public:
    UFUNCTION()
    void OnReceivePocket(FName Pocket);

    FName CurrentPocket;
};
