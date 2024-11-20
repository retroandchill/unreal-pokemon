// "Unreal Pokémon" created by Retro & Chill.

#include "ScopedFunctionalTest.h"
#include "ScopedTestComponent.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Casting/InstanceOf.h"
#include "Ranges/Casting/StaticCast.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/ContainerView.h"

void AScopedFunctionalTest::PrepareTest() {
    Super::PrepareTest();
    // clang-format off
    GetComponents() |
        UE::Ranges::Filter(UE::Ranges::InstanceOf<UScopedTestComponent>) |
        UE::Ranges::Map(UE::Ranges::StaticCast<UScopedTestComponent *>) |
        UE::Ranges::ForEach(&UScopedTestComponent::AddScopedComponent, this);
    // clang-format on
    AddScopedTestObjects();
}

void AScopedFunctionalTest::FinishTest(EFunctionalTestResult TestResult, const FString &Message) {
    Super::FinishTest(TestResult, Message);
    ScopedData.Empty();
}

void AScopedFunctionalTest::AddScopedTestObjects() {
    // No base functionality
}