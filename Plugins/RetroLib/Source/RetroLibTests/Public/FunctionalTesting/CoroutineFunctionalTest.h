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

/**
 * ACoroutineFunctionalTest is an abstract class that extends AFunctionalTest
 * and provides functionality for running asynchronous coroutine-based tests.
 * It is designed to integrate with the UE5Coro library for Unreal Engine
 * coroutine management.
 */
UCLASS(Abstract)
class RETROLIBTESTS_API ACoroutineFunctionalTest : public AFunctionalTest {
    GENERATED_BODY()

#if RETROLIB_WITH_UE5CORO
  public:
    void StartTest() final;

  protected:
    /**
     * Runs the test as an asynchronous coroutine. This method must be implemented by
     * derived classes to provide specific functionality for the test. It is designed
     * to integrate with the UE5Coro library, allowing for coroutine-based testing
     * workflows.
     *
     * @param Coro A force latent coroutine parameter used to control coroutine execution.
     * @return A coroutine object signifying the execution of the test.
     */
    virtual UE5Coro::TCoroutine<> RunTest(FForceLatentCoroutine Coro = {}) ABSTRACT_METHOD
#endif
};