#include "Abilities/GameplayAbility.h"
#include "Asserts.h"
#include "Battle/Items/ItemLookup.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestHoldItemLookup, "Unit Tests.Battle.Items.TestHoldItemLookup",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestHoldItemLookup::RunTest(const FString &Parameters)
{
    auto ItemClass = Pokemon::Battle::Items::FindHoldItemEffect("MUSCLEBAND");
    UE_CHECK_NOT_NULL(ItemClass.GetPtrOrNull());
    UE_CHECK_EQUAL(TEXT("HoldItem_MUSCLEBAND_C"), (*ItemClass)->GetName());

    ItemClass = Pokemon::Battle::Items::FindHoldItemEffect("NotARealHoldItemEffect");
    UE_CHECK_NULL(ItemClass.GetPtrOrNull());

    ItemClass = Pokemon::Battle::Items::FindHoldItemEffect(NAME_None);
    UE_CHECK_NULL(ItemClass.GetPtrOrNull());
    return true;
}