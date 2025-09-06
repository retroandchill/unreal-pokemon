// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/PokemonDTO.h"
#include "RetroLibTests/Public/FunctionalTesting/CoroutineFunctionalTest.h"

#include "GivePokemonItemTest.generated.h"

UCLASS(BlueprintType)
class POKEMONTESTS_API AGivePokemonItemTest : public ACoroutineFunctionalTest
{
    GENERATED_BODY()

  public:
    AGivePokemonItemTest();

  protected:
    UE5Coro::TCoroutine<> RunTest(FForceLatentCoroutine) override;
};