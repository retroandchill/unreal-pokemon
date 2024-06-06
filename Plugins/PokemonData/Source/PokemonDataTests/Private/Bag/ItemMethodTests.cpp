#include "Asserts.h"
#include "Bag/Item.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemMethodTests, "Private.Bag.ItemMethodTests",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemMethodTests::RunTest(const FString &Parameters) {
    FItem FakeItem;

    // Portion Name
    FakeItem.RealName = FText::FromStringView(TEXT("Fake Item"));
    CHECK_EQUAL(TEXT("Fake Item"), FakeItem.GetPortionName().ToString());
    FakeItem.RealPortionName = FText::FromStringView(TEXT("Fake Portion"));
    CHECK_EQUAL(TEXT("Fake Portion"), FakeItem.GetPortionName().ToString());

    // TM/HM/TR
    FakeItem.FieldUse = EFieldUse::Direct;
    CHECK_FALSE(FakeItem.IsTM());
    CHECK_FALSE(FakeItem.IsHM());
    CHECK_FALSE(FakeItem.IsTR());
    FakeItem.FieldUse = EFieldUse::TM;
    CHECK_TRUE(FakeItem.IsTM());
    FakeItem.FieldUse = EFieldUse::HM;
    CHECK_TRUE(FakeItem.IsHM());
    FakeItem.FieldUse = EFieldUse::TR;
    CHECK_TRUE(FakeItem.IsTR());

    // Types of Item
    FakeItem.Tags.Empty();
    CHECK_FALSE(FakeItem.IsPokeBall());
    CHECK_FALSE(FakeItem.IsMail());
    FakeItem.Tags = { TEXT("PokeBall") };
    CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = { TEXT("SnagBall") };
    CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = { TEXT("PokeBall"), TEXT("SnagBall") };
    CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = { TEXT("Mail") };
    CHECK_TRUE(FakeItem.IsMail());
    FakeItem.Tags = { TEXT("IconMail") };
    CHECK_TRUE(FakeItem.IsMail());
    FakeItem.Tags = { TEXT("Mail"), TEXT("IconMail") };
    CHECK_TRUE(FakeItem.IsMail());
    
    return true;
}