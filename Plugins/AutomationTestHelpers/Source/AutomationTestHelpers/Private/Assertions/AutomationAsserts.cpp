// "Unreal Pokémon" created by Retro & Chill.


#include "Assertions/AutomationAsserts.h"

template <>
bool AssertTrue<FAutomationTestBase>(FAutomationTestBase &TestObject, FStringView What, bool Condition) {
    return TestObject.TestTrue(What.GetData(), Condition);
}

template <>
bool AssertFalse<FAutomationTestBase>(FAutomationTestBase &TestObject, FStringView What, bool Condition) {
    return TestObject.TestFalse(What.GetData(), Condition);
}

bool AssertEqual(FAutomationTestBase &TestObject, FStringView What, const FStringView &Expected,
    const FStringView &Actual) {
    return TestObject.TestEqual(What.GetData(), Actual.GetData(), Expected.GetData());
}

auto ConcludeTest(FAutomationTestBase &, bool bSuccess) {
    return bSuccess;
}