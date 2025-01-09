// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Utils/Unreachable.h"

#if RETROLIB_WITH_UE5CORO
#include "UE5Coro.h"
#endif

#include "CoroutineFunctionalTest.generated.h"

UCLASS(Abstract)
class RETROLIBTESTS_API ACoroutineFunctionalTest : public AFunctionalTest {
    GENERATED_BODY()

#if RETROLIB_WITH_UE5CORO
  public:
    void StartTest() final;

  protected:
    virtual UE5Coro::TCoroutine<> RunTest(FForceLatentCoroutine Coro = {}) ABSTRACT_METHOD
#endif
};