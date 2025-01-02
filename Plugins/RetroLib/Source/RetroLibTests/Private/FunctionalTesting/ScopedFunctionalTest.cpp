// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTesting/ScopedFunctionalTest.h"
#include "FunctionalTesting/ScopedTestComponent.h"
#include "RetroLib/Casting/InstanceOf.h"
#include "RetroLib/Casting/StaticCast.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Compatibility/Array.h"





void AScopedFunctionalTest::PrepareTest() {
    Super::PrepareTest();
    // clang-format off
    GetComponents() |
        Retro::Ranges::Views::Filter(Retro::InstanceOf<UScopedTestComponent>) |
        Retro::Ranges::Views::Transform(Retro::StaticCast<UScopedTestComponent *>) |
        Retro::Ranges::ForEach(&UScopedTestComponent::AddScopedComponent, std::ref(*this));
    // clang-format on
}

void AScopedFunctionalTest::FinishTest(EFunctionalTestResult TestResult, const FString &Message) {
    Super::FinishTest(TestResult, Message);

    // This will force all the destructors of the scoped components to be called
    ScopedData.Empty();
}