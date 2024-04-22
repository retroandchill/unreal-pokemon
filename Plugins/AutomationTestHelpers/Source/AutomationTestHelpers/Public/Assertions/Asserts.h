// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Assert that the given statement is true.
 * @tparam TestClass The test class.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Condition The condition that is being evaluated.
 * @return Did the assert succeed?
 */
template <typename TestClass>
bool AssertTrue(TestClass &TestObject, FStringView What, bool Condition);

/**
 * Assert that the given statement is false.
 * @tparam TestClass The test class.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Condition The condition that is being evaluated.
 * @return Did the assert succeed?
 */
template <typename TestClass>
bool AssertFalse(TestClass &TestObject, FStringView What, bool Condition);

/**
 * Assert that the given pointer is null.
 * @tparam TestClass The test class.
 * @tparam PointerType The type of pointer being evaluated
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Pointer The pointer to evaluate
 * @return Did the assert succeed?
 */
template <typename TestClass, typename PointerType>
requires std::is_pointer_v<PointerType>
bool AssertNull(TestClass &TestObject, FStringView What, PointerType Pointer);

/**
 * Assert that the given pointer is not null.
 * @tparam TestClass The test class.
 * @tparam PointerType The type of pointer being evaluated
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Pointer The pointer to evaluate
 * @return Did the assert succeed?
 */
template <typename TestClass, typename PointerType>
requires std::is_pointer_v<PointerType>
bool AssertNotNull(TestClass &TestObject, FStringView What, PointerType Pointer);

/**
 * Assert that the two given values are equal?
 * @tparam TestClass he test class.
 * @tparam ValueType The type of data that is being compared.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
template <typename TestClass, typename ValueType>
bool AssertEqual(TestClass &TestObject, FStringView What, const ValueType& Expected, const ValueType& Actual);

/**
 * Assert that the two given values are not equal?
 * @tparam TestClass The test class.
 * @tparam ValueType The type of data that is being compared.
 * @param TestObject The test object.
 * @param What The string representation of what's being tested.
 * @param Expected The value to compare the test result to.
 * @param Actual The test result.
 * @return Did the assert succeed?
 */
template <typename TestClass, typename ValueType>
bool AssertNotEqual(TestClass &TestObject, FStringView What, const ValueType& Expected, const ValueType& Actual);

#define CHECK_TRUE(Condition) AssertTrue(*this, TEXT(#Condition), Condition)
#define CHECK_FALSE(Condition) AssertFalse(*this, TEXT(#Condition), Condition)
#define CHECK_NULL(Pointer) AssertNull(*this, TEXT(#Pointer), Pointer)
#define CHECK_NOT_NULL(Pointer) AssertNotNull(*this, TEXT(#Pointer), Pointer)
#define CHECK_EQUAL(Expected, Actual) AssertEqual(*this, TEXT(#Actual), Expected, Actual)
#define CHECK_NOT_EQUAL(Expected, Actual) AssertNotEqual(*this, TEXT(#Actual), Expected, Actual)

#define ASSERT_TRUE(Condition) if (!CHECK_TRUE(Condition)) return ConcludeTest(*this, false)
#define ASSERT_FALSE(Condition) if (!CHECK_FALSE(Condition)) return ConcludeTest(*this, false)
#define ASSERT_NULL(Pointer) if (!CHECK_NULL(Pointer)) return ConcludeTest(*this, false)
#define ASSERT_NOT_NULL(Pointer) if (!CHECK_NOT_NULL(Pointer)) return ConcludeTest(*this, false)
#define ASSERT_EQUAL(Expected, Actual) if (!CHECK_EQUAL(Expected, Actual)) return ConcludeTest(*this, false)
#define ASSERT_NOT_EQUAL(Expected, Actual) if (!CHECK_NOT_EQUAL(Expected, Actual)) return ConcludeTest(*this, false)