#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Functional/TryInvoke.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTryInvoke, "Unit Tests.Ranges.Functional.TestTryInvoke",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

int32 Add(int32 A, int32 B) {
    return A + B;
}

bool TestTryInvoke::RunTest(const FString &Parameters) {
    UE_CHECK_EQUAL(4, UE::Ranges::TryInvoke(&Add, 2, 2));
    UE_CHECK_EQUAL(4, UE::Ranges::TryInvoke(&Add, std::make_pair(2, 2)));
    return true;
}