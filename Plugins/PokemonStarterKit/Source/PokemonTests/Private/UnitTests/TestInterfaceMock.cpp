#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Pokemon.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestInterfaceMock, "Unit Tests.Core.TestInterfaceMock",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestInterfaceMock::RunTest(const FString &Parameters) {
    auto [Pokemon, Mock] = UnrealMock::CreateMock<IPokemon>();
    ON_CALL(Mock, GetPersonalityValue).Return(3);
    UE_CHECK_EQUAL(3, Pokemon->GetPersonalityValue());

    return true;
}