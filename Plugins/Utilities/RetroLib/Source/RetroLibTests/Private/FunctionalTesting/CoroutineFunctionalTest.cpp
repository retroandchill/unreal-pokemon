// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTesting/CoroutineFunctionalTest.h"

void ACoroutineFunctionalTest::StartTest() {
    [](UE5Coro::TLatentContext<ACoroutineFunctionalTest> This) -> UE5Coro::TCoroutine<> {
        co_await This->RunTest();
        This->FinishTest(EFunctionalTestResult::Default, TEXT("Test Complete"));
    }(this);
}