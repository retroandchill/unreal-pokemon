// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "NoItemSelectedDispatcher.generated.h"

/**
 *
 */
UCLASS()
class POKEMONUITESTS_API UNoItemSelectedDispatcher : public UObject {
    GENERATED_BODY()

  public:
    UFUNCTION()
    void OnReceive();

    bool bCalled = false;
};
