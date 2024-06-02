#include <array>

#include "Asserts.h"
#include "Mainpulation/CircularIterator.h"
#include "Misc/AutomationTest.h"

struct FTestStruct {
    int32 Value;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CircularIteratorTest, "Unit Tests.Utilities.CircularIteratorTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CircularIteratorTest::RunTest(const FString &Parameters) {
    TCircularIterator<int32> Iterator;
    CHECK_FALSE(Iterator.IsValid());
    CHECK_FALSE(Iterator.CanCycle());

    TArray Data = {1, 2, 3};
    Iterator = TCircularIterator<int32>(Data);
    CHECK_TRUE(Iterator.IsValid());
    CHECK_TRUE(Iterator.CanCycle());

    // Check the prefix operators
    CHECK_EQUAL(1, *Iterator);
    ++Iterator;
    CHECK_EQUAL(2, *Iterator);
    ++Iterator;
    CHECK_EQUAL(3, *Iterator);
    ++Iterator;
    CHECK_EQUAL(1, *Iterator);
    --Iterator;
    CHECK_EQUAL(3, *Iterator);
    --Iterator;
    CHECK_EQUAL(2, *Iterator);
    --Iterator;
    CHECK_EQUAL(1, *Iterator);
    --Iterator;
    CHECK_EQUAL(3, *Iterator);

    // Check the postfix operators
    auto IteratorCopy = Iterator++;
    CHECK_EQUAL(3, *IteratorCopy);
    CHECK_EQUAL(1, *Iterator);
    IteratorCopy = Iterator--;
    CHECK_EQUAL(1, *IteratorCopy);
    CHECK_EQUAL(3, *Iterator);

    // Check the += and -= operators
    Iterator += 2;
    CHECK_EQUAL(2, *Iterator);
    Iterator += 3;
    CHECK_EQUAL(2, *Iterator);
    Iterator += 5;
    CHECK_EQUAL(1, *Iterator);
    Iterator -= 5;
    CHECK_EQUAL(2, *Iterator);

    Iterator -= 2;
    CHECK_EQUAL(3, *Iterator);
    Iterator -= 7;
    CHECK_EQUAL(2, *Iterator);
    Iterator -= -2;
    CHECK_EQUAL(1, *Iterator);

    // Check that when we dereference a complex value we get a valid pointer
    TStaticArray<FTestStruct, 3> StructArray;
    StructArray[0].Value = 1;
    StructArray[1].Value = 2;
    StructArray[2].Value = 3;
    TCircularIterator<FTestStruct> StructIterator(StructArray, 1);
    CHECK_EQUAL(2, StructIterator->Value);

    return true;
}