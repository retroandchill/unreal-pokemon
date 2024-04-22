// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Asserts.h"

template <>
AUTOMATIONTESTHELPERS_API bool AssertTrue(FAutomationTestBase &TestObject, FStringView What, bool Condition);

template <>
AUTOMATIONTESTHELPERS_API bool AssertFalse(FAutomationTestBase &TestObject, FStringView What, bool Condition);

template <typename PointerType>
requires std::is_pointer_v<PointerType>
bool AssertNull(FAutomationTestBase &TestObject, FStringView What, PointerType Pointer) {
    return TestObject.TestNull(What.GetData(), Pointer);
}

template <typename PointerType>
requires std::is_pointer_v<PointerType>
bool AssertNotNull(FAutomationTestBase &TestObject, FStringView What, PointerType Pointer) {
    return TestObject.TestNotNull(What.GetData(), Pointer);
}

template <typename ValueType>
bool AssertEqual(FAutomationTestBase &TestObject, FStringView What, const ValueType& Expected, const ValueType& Actual) {
    return TestObject.TestEqual(What.GetData(), Actual, Expected);
}

AUTOMATIONTESTHELPERS_API bool AssertEqual(FAutomationTestBase &TestObject, FStringView What, const FStringView& Expected, const FStringView& Actual);

template <typename ValueType>
bool AssertNotEqual(FAutomationTestBase &TestObject, FStringView What, const ValueType& Expected, const ValueType& Actual) {
    return TestObject.TestNotEqual(What.GetData(), Actual, Expected);
}

/**
 * Conclude the test case.
 * @param TestObject The test object.
 * @param bSuccess Did the test succeed.
 * @return The necessary return value of the test.
 */
auto ConcludeTest(FAutomationTestBase &TestObject, bool bSuccess);
