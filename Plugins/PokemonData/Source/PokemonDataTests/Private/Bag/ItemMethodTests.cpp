#include "Asserts.h"
#include "Bag/Item.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemMethodTests, "Unit Tests.Data.Bag.ItemMethodTests",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemMethodTests::RunTest(const FString &Parameters) {
    FItem FakeItem;
    
    FakeItem.ShowQuantity = false;
    CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.ShowQuantity = true;
    CHECK_TRUE(FakeItem.ShouldShowQuantity());

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
    CHECK_FALSE(FakeItem.CanHold());
    CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.FieldUse = EFieldUse::HM;
    CHECK_TRUE(FakeItem.IsHM());
    CHECK_FALSE(FakeItem.CanHold());
    CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.FieldUse = EFieldUse::TR;
    CHECK_TRUE(FakeItem.IsTR());
    CHECK_TRUE(FakeItem.CanHold());
    CHECK_TRUE(FakeItem.ShouldShowQuantity());

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
    CHECK_TRUE(UItemHelper::IsMail(FakeItem));

    // Key Item/Is Important
    FakeItem.Tags.Empty();
    CHECK_FALSE(FakeItem.IsKeyItem());
    CHECK_FALSE(FakeItem.IsImportant());
    CHECK_TRUE(FakeItem.CanHold());
    CHECK_TRUE(FakeItem.ShouldShowQuantity());
    FakeItem.Tags = { TEXT("KeyItem") };
    CHECK_TRUE(FakeItem.IsKeyItem());
    CHECK_TRUE(FakeItem.IsImportant());
    CHECK_FALSE(FakeItem.CanHold());
    CHECK_FALSE(FakeItem.ShouldShowQuantity());
    CHECK_FALSE(UItemHelper::CanHold(FakeItem));
    
    
    return true;
}