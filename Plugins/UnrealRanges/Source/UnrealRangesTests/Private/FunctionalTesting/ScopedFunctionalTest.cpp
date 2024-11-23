// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTesting/ScopedFunctionalTest.h"
#include "FunctionalTesting/ScopedTestComponent.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Casting/InstanceOf.h"
#include "Ranges/Casting/StaticCast.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"

void AScopedFunctionalTest::PrepareTest() {
    Super::PrepareTest();
    // clang-format off
    GetComponents() |
        UE::Ranges::Filter(UE::Ranges::InstanceOf<UScopedTestComponent>) |
        UE::Ranges::Map(UE::Ranges::StaticCast<UScopedTestComponent *>) |
        UE::Ranges::ForEach(&UScopedTestComponent::AddScopedComponent, REF(*this));
    // clang-format on
}

void AScopedFunctionalTest::FinishTest(EFunctionalTestResult TestResult, const FString &Message) {
    Super::FinishTest(TestResult, Message);

    // This will force all the destructors of the scoped components to be called
    ScopedData.Empty();
}