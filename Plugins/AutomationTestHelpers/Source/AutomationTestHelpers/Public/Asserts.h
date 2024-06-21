﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#define UE_CHECK_TRUE(Condition) AssertTrue(*this, TEXT(#Condition), Condition)
#define UE_CHECK_FALSE(Condition) AssertFalse(*this, TEXT(#Condition), Condition)
#define UE_CHECK_NULL(Pointer) AssertNull(*this, TEXT(#Pointer), Pointer)
#define UE_CHECK_NOT_NULL(Pointer) AssertNotNull(*this, TEXT(#Pointer), Pointer)
#define UE_CHECK_EQUAL(Expected, Actual) AssertEqual(*this, TEXT(#Actual), Expected, Actual)
#define UE_CHECK_NOT_EQUAL(Expected, Actual) AssertNotEqual(*this, TEXT(#Actual), Expected, Actual)

#define UE_ASSERT_TRUE(Condition)                                                                                         \
    if (!UE_CHECK_TRUE(Condition))                                                                                        \
    return ConcludeTest(*this, false)
#define UE_ASSERT_FALSE(Condition)                                                                                        \
    if (!UE_CHECK_FALSE(Condition))                                                                                       \
    return ConcludeTest(*this, false)
#define UE_ASSERT_NULL(Pointer)                                                                                           \
    if (!UE_CHECK_NULL(Pointer))                                                                                          \
    return ConcludeTest(*this, false)
#define UE_ASSERT_NOT_NULL(Pointer)                                                                                       \
    if (!UE_CHECK_NOT_NULL(Pointer))                                                                                      \
    return ConcludeTest(*this, false)
#define UE_ASSERT_EQUAL(Expected, Actual)                                                                                 \
    if (!UE_CHECK_EQUAL(Expected, Actual))                                                                                \
    return ConcludeTest(*this, false)
#define UE_ASSERT_NOT_EQUAL(Expected, Actual)                                                                             \
    if (!UE_CHECK_NOT_EQUAL(Expected, Actual))                                                                            \
    return ConcludeTest(*this, false)

/**
 * Assert that the given statement is true.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Condition The condition that is being evaluated.
 * @return Did the assert succeed?
 */
FORCEINLINE bool AssertTrue(FAutomationTestBase &TestObject, FStringView What, bool Condition) {
    return TestObject.TestTrue(What.GetData(), Condition);
}

/**
 * Assert that the given statement is false.
 * @tparam TestClass The test class.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Condition The condition that is being evaluated.
 * @return Did the assert succeed?
 */
FORCEINLINE bool AssertFalse(FAutomationTestBase &TestObject, FStringView What, bool Condition) {
    return TestObject.TestFalse(What.GetData(), Condition);
}

/**
 * Assert that the given pointer is null.
 * @tparam PointerType The type of pointer being evaluated
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Pointer The pointer to evaluate
 * @return Did the assert succeed?
 */
template <typename PointerType>
    requires std::is_pointer_v<PointerType>
FORCEINLINE bool AssertNull(FAutomationTestBase &TestObject, FStringView What, PointerType Pointer) {
    return TestObject.TestNull(What.GetData(), Pointer);
}

/**
 * Assert that the given pointer is not null.
 * @tparam PointerType The type of pointer being evaluated
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Pointer The pointer to evaluate
 * @return Did the assert succeed?
 */
template <typename PointerType>
    requires std::is_pointer_v<PointerType>
FORCEINLINE bool AssertNotNull(FAutomationTestBase &TestObject, FStringView What, PointerType Pointer) {
    return TestObject.TestNotNull(What.GetData(), Pointer);
}

/**
 * Assert that the two given values are equal?
 * @tparam A The type of data the first variable that is being compared.
 * @tparam B The type of data the second variable that is being compared.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
template <typename A, typename B>
FORCEINLINE bool AssertEqual(FAutomationTestBase &TestObject, FStringView What, const A &Expected, const B &Actual) {
    return TestObject.TestEqual(What.GetData(), Actual, Expected);
}

/**
 * Assert that the two given values are equal?
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
FORCEINLINE bool AssertEqual(FAutomationTestBase &TestObject, FStringView What, FStringView Expected,
                             FStringView Actual) {
    return TestObject.TestEqual(What.GetData(), Actual.GetData(), Expected.GetData());
}

/**
 * Assert that the two given values are not equal?
 * @tparam ValueType The type of data that is being compared.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
template <typename ValueType>
FORCEINLINE bool AssertNotEqual(FAutomationTestBase &TestObject, FStringView What, const ValueType &Expected,
                                const ValueType &Actual) {
    return TestObject.TestNotEqual(What.GetData(), Actual, Expected);
}

/**
 * Assert that the two given values are not equal?
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
FORCEINLINE bool AssertNotEqual(FAutomationTestBase &TestObject, FStringView What, FStringView Expected,
                                FStringView Actual) {
    return TestObject.TestEqual(What.GetData(), Actual.GetData(), Expected.GetData());
}

/**
 * Conclude the test case.
 * @param TestObject The test object.
 * @param bSuccess Did the test succeed.
 * @return The necessary return value of the test.
 */
FORCEINLINE bool ConcludeTest(FAutomationTestBase &TestObject, bool bSuccess) {
    return bSuccess;
}
