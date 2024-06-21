#include "Asserts.h"
#include "Manipulation/CircularIterator.h"
#include "Misc/AutomationTest.h"
#include <array>

struct FTestStruct {
    int32 Value;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CircularIteratorTest, "Unit Tests.Utilities.CircularIteratorTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CircularIteratorTest::RunTest(const FString &Parameters) {
    TCircularIterator<int32> Iterator;
    UE_CHECK_FALSE(Iterator.IsValid());
    UE_CHECK_FALSE(Iterator.CanCycle());

    TArray Data = {1, 2, 3};
    Iterator = TCircularIterator<int32>(Data);
    UE_CHECK_TRUE(Iterator.IsValid());
    UE_CHECK_TRUE(Iterator.CanCycle());

    // Check the prefix operators
    UE_CHECK_EQUAL(1, *Iterator);
    ++Iterator;
    UE_CHECK_EQUAL(2, *Iterator);
    ++Iterator;
    UE_CHECK_EQUAL(3, *Iterator);
    ++Iterator;
    UE_CHECK_EQUAL(1, *Iterator);
    --Iterator;
    UE_CHECK_EQUAL(3, *Iterator);
    --Iterator;
    UE_CHECK_EQUAL(2, *Iterator);
    --Iterator;
    UE_CHECK_EQUAL(1, *Iterator);
    --Iterator;
    UE_CHECK_EQUAL(3, *Iterator);

    // Check the postfix operators
    auto IteratorCopy = Iterator++;
    UE_CHECK_EQUAL(3, *IteratorCopy);
    UE_CHECK_EQUAL(1, *Iterator);
    IteratorCopy = Iterator--;
    UE_CHECK_EQUAL(1, *IteratorCopy);
    UE_CHECK_EQUAL(3, *Iterator);

    // Check the += and -= operators
    Iterator += 2;
    UE_CHECK_EQUAL(2, *Iterator);
    Iterator += 3;
    UE_CHECK_EQUAL(2, *Iterator);
    Iterator += 5;
    UE_CHECK_EQUAL(1, *Iterator);
    Iterator -= 5;
    UE_CHECK_EQUAL(2, *Iterator);

    Iterator -= 2;
    UE_CHECK_EQUAL(3, *Iterator);
    Iterator -= 7;
    UE_CHECK_EQUAL(2, *Iterator);
    Iterator -= -2;
    UE_CHECK_EQUAL(1, *Iterator);

    // Check that when we dereference a complex value we get a valid pointer
    TStaticArray<FTestStruct, 3> StructArray;
    StructArray[0].Value = 1;
    StructArray[1].Value = 2;
    StructArray[2].Value = 3;
    TCircularIterator<FTestStruct> StructIterator(StructArray, 1);
    UE_CHECK_EQUAL(2, StructIterator->Value);

    return true;
}