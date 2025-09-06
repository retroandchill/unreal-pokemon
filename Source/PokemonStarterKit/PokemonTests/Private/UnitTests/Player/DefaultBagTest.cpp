
#include "Player/DefaultBag.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Player/Sorting/IndexedBagSorter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagTest, "Unit Tests.Player.BagTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagTest::RunTest(const FString &Parameters)
{
    auto Bag = NewObject<UDefaultBag>();
    UE_CHECK_EQUAL(2, Bag->ObtainItem(TEXT("FULLHEAL"), 2));
    UE_CHECK_EQUAL(5, Bag->ObtainItem(TEXT("POTION"), 5));
    UE_CHECK_EQUAL(3, Bag->ObtainItem(TEXT("POTION"), 3));
    UE_CHECK_EQUAL(20, Bag->ObtainItem(TEXT("ORANBERRY"), 20));

    UE_CHECK_EQUAL(2, Bag->GetItemQuantity(TEXT("FULLHEAL")));
    UE_CHECK_EQUAL(8, Bag->GetItemQuantity(TEXT("POTION")));
    UE_CHECK_EQUAL(20, Bag->GetItemQuantity(TEXT("ORANBERRY")));

    FIndexedBagSorter Sorter;
    Bag->SortPocket(TEXT("Medicine"), Sorter);
    TArray<FString> PocketContents;
    Bag->ForEachInPocket(TEXT("Medicine"),
                         [&PocketContents](FName Item, int32) { PocketContents.Add(Item.ToString()); });
    UE_ASSERT_EQUAL(2, PocketContents.Num());
    UE_CHECK_EQUAL(TEXT("POTION"), PocketContents[0]);
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), PocketContents[1]);

    UE_CHECK_EQUAL(1, Bag->RemoveItem(TEXT("POTION")));
    UE_CHECK_EQUAL(2, Bag->RemoveItem(TEXT("FULLHEAL"), 4));
    UE_CHECK_EQUAL(0, Bag->RemoveItem(TEXT("POKEBALL")));

    return true;
}