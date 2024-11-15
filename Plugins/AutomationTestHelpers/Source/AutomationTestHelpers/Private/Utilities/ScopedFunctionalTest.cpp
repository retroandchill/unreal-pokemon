// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/ScopedFunctionalTest.h"


void AScopedFunctionalTest::PrepareTest() {
    Super::PrepareTest();
    AddScopedTestObjects();
}

void AScopedFunctionalTest::FinishTest(EFunctionalTestResult TestResult, const FString &Message) {
    Super::FinishTest(TestResult, Message);
    ScopedData.Empty();
}

void AScopedFunctionalTest::AddScopedTestObjects() {
    // No base functionality
}