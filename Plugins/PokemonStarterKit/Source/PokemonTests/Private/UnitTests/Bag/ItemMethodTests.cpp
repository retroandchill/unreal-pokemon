#include "Asserts.h"
#include "Bag/Item.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemMethodTests, "Unit Tests.Data.Bag.ItemMethodTests",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemMethodTests::RunTest(const FString &Parameters) {
    FItem FakeItem;

    FakeItem.ShowQuantity = false;
    UE_CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.ShowQuantity = true;
    UE_CHECK_TRUE(FakeItem.ShouldShowQuantity());

    // Portion Name
    FakeItem.RealName = FText::FromStringView(TEXT("Fake Item"));
    UE_CHECK_EQUAL(TEXT("Fake Item"), FakeItem.GetPortionName().ToString());
    UE_CHECK_EQUAL(TEXT("Fake Item"), UItemHelper::GetPortionName(FakeItem).ToString());
    FakeItem.RealPortionName = FText::FromStringView(TEXT("Fake Portion"));
    UE_CHECK_EQUAL(TEXT("Fake Portion"), FakeItem.GetPortionName().ToString());
    UE_CHECK_EQUAL(TEXT("Fake Portion"), UItemHelper::GetPortionName(FakeItem).ToString());

    // TM/HM/TR
    FakeItem.FieldUse = EFieldUse::Direct;
    UE_CHECK_FALSE(FakeItem.IsTM());
    UE_CHECK_FALSE(FakeItem.IsHM());
    UE_CHECK_FALSE(FakeItem.IsTR());
    FakeItem.FieldUse = EFieldUse::TM;
    UE_CHECK_TRUE(FakeItem.IsTM());
    UE_CHECK_FALSE(FakeItem.CanHold());
    UE_CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.FieldUse = EFieldUse::HM;
    UE_CHECK_TRUE(FakeItem.IsHM());
    UE_CHECK_FALSE(FakeItem.CanHold());
    UE_CHECK_FALSE(FakeItem.ShouldShowQuantity());
    FakeItem.FieldUse = EFieldUse::TR;
    UE_CHECK_TRUE(FakeItem.IsTR());
    UE_CHECK_TRUE(FakeItem.CanHold());
    UE_CHECK_TRUE(FakeItem.ShouldShowQuantity());

    // Types of Item
    FakeItem.Tags.Empty();
    UE_CHECK_FALSE(FakeItem.IsPokeBall());
    UE_CHECK_FALSE(FakeItem.IsMail());
    FakeItem.Tags = {TEXT("PokeBall")};
    UE_CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = {TEXT("SnagBall")};
    UE_CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = {TEXT("PokeBall"), TEXT("SnagBall")};
    UE_CHECK_TRUE(FakeItem.IsPokeBall());
    FakeItem.Tags = {TEXT("Mail")};
    UE_CHECK_TRUE(FakeItem.IsMail());
    FakeItem.Tags = {TEXT("IconMail")};
    UE_CHECK_TRUE(FakeItem.IsMail());
    FakeItem.Tags = {TEXT("Mail"), TEXT("IconMail")};
    UE_CHECK_TRUE(FakeItem.IsMail());
    UE_CHECK_TRUE(UItemHelper::IsMail(FakeItem));

    // Key Item/Is Important
    FakeItem.Tags.Empty();
    UE_CHECK_FALSE(FakeItem.IsKeyItem());
    UE_CHECK_FALSE(FakeItem.IsImportant());
    UE_CHECK_TRUE(FakeItem.CanHold());
    UE_CHECK_TRUE(FakeItem.ShouldShowQuantity());
    FakeItem.Tags = {TEXT("KeyItem")};
    UE_CHECK_TRUE(FakeItem.IsKeyItem());
    UE_CHECK_TRUE(FakeItem.IsImportant());
    UE_CHECK_FALSE(FakeItem.CanHold());
    UE_CHECK_FALSE(FakeItem.ShouldShowQuantity());
    UE_CHECK_FALSE(UItemHelper::CanHold(FakeItem));

    return true;
}