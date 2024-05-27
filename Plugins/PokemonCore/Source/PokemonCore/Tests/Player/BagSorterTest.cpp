#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Player/ItemSlot.h"
#include "Player/Sorting/IndexedBagSorter.h"
#include "Player/Sorting/NamedBagSorter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagSorterTest_Indexed, "Tests.Player.BagSorterTest.IndexedBagSorter",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagSorterTest_Indexed::RunTest(const FString &Parameters) {
    TArray<FItemSlot, TInlineAllocator<4>> Pocket = {
        {.Item = TEXT("SUPERPOTION")},
        {.Item = TEXT("POTION")},
        {.Item = TEXT("MAXPOTION")},
        {.Item = TEXT("HYPERPOTION")}
    };

    FIndexedBagSorter Sorter;
    Sorter.SortPocket(Pocket);
    ASSERT_EQUAL(TEXT("POTION"), Pocket[0].Item.ToString());
    ASSERT_EQUAL(TEXT("SUPERPOTION"), Pocket[1].Item.ToString());
    ASSERT_EQUAL(TEXT("HYPERPOTION"), Pocket[2].Item.ToString());
    ASSERT_EQUAL(TEXT("MAXPOTION"), Pocket[3].Item.ToString());
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagSorterTest_Named, "Tests.Player.BagSorterTest.NamedBagSorter",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagSorterTest_Named::RunTest(const FString &Parameters) {
    TArray<FItemSlot, TInlineAllocator<4>> Pocket = {
        {.Item = TEXT("SUPERPOTION")},
        {.Item = TEXT("POTION")},
        {.Item = TEXT("MAXPOTION")},
        {.Item = TEXT("HYPERPOTION")}
    };

    FNamedBagSorter Sorter;
    Sorter.SortPocket(Pocket);
    ASSERT_EQUAL(TEXT("HYPERPOTION"), Pocket[0].Item.ToString());
    ASSERT_EQUAL(TEXT("MAXPOTION"), Pocket[1].Item.ToString());
    ASSERT_EQUAL(TEXT("POTION"), Pocket[2].Item.ToString());
    ASSERT_EQUAL(TEXT("SUPERPOTION"), Pocket[3].Item.ToString());
    
    return true;
}
#endif