#include "Misc/AutomationTest.h"
#include "RetroLib/Ranges/CircularIterator.h"
#include "TestAdapter.h"
#include <array>

namespace Retro::Testing {
    struct FMyTestStruct {
        int32 Value;
    };
} // namespace Retro::Testing

TEST_CASE_NAMED(FCircularIteratorTest, "Unit Tests::RetroLib::Ranges::CircularIterator", "[RetroLib][Ranges]") {
    using namespace Retro::Testing;
    Retro::TCircularIterator<int32> Iterator;
    CHECK_FALSE(Iterator.IsValid());
    CHECK_FALSE(Iterator.CanCycle());

    TArray Data = {1, 2, 3};
    Iterator = Retro::TCircularIterator<int32>(Data);
    CHECK(Iterator.IsValid());
    CHECK(Iterator.CanCycle());

    // Check the prefix operators
    CHECK(*Iterator == 1);
    ++Iterator;
    CHECK(*Iterator == 2);
    ++Iterator;
    CHECK(*Iterator == 3);
    ++Iterator;
    CHECK(*Iterator == 1);
    --Iterator;
    CHECK(*Iterator == 3);
    --Iterator;
    CHECK(*Iterator == 2);
    --Iterator;
    CHECK(*Iterator == 1);
    --Iterator;
    CHECK(*Iterator == 3);

    // Check the postfix operators
    auto IteratorCopy = Iterator++;
    CHECK(*IteratorCopy == 3);
    CHECK(*Iterator == 1);
    IteratorCopy = Iterator--;
    CHECK(*IteratorCopy == 1);
    CHECK(*Iterator == 3);

    // Check the += and -= operators
    Iterator += 2;
    CHECK(*Iterator == 2);
    Iterator += 3;
    CHECK(*Iterator == 2);
    Iterator += 5;
    CHECK(*Iterator == 1);
    Iterator -= 5;
    CHECK(*Iterator == 2);

    Iterator -= 2;
    CHECK(*Iterator == 3);
    Iterator -= 7;
    CHECK(*Iterator == 2);
    Iterator -= -2;
    CHECK(*Iterator == 1);

    // Check that when we dereference a complex value we get a valid pointer
    std::array<FMyTestStruct, 3> StructArray;
    StructArray[0].Value = 1;
    StructArray[1].Value = 2;
    StructArray[2].Value = 3;
    Retro::TCircularIterator<FMyTestStruct> StructIterator(StructArray, 1);
    CHECK(StructIterator->Value, 2);
}