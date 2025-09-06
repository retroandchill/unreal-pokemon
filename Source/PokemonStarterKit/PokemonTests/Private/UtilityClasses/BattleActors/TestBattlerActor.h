// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battlers/BattlerActor.h"

#include "TestBattlerActor.generated.h"

UCLASS(BlueprintInternalUseOnly)
class POKEMONTESTS_API ATestBattlerActor : public ABattlerActor
{
    GENERATED_BODY()

  public:
    ATestBattlerActor();
    void PostActorCreated() override;
    UE5Coro::TCoroutine<> SpawnSpriteActor(bool ShouldShow) override;
};