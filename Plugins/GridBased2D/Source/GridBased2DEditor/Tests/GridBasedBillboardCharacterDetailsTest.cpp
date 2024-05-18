#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Details/GridBasedBillboardCharacterDetails.h"
#include "Asserts.h"
#include "DetailLayoutBuilder.h"
#include "Misc/AutomationTest.h"
#include "External/fakeit.hpp"
#include "DetailCategoryBuilder.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridBasedBillboardCharacterDetailsTest, "Tests.GridBasedBillboardCharacterDetailsTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GridBasedBillboardCharacterDetailsTest::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.
    auto Customization = FGridBasedBillboardCharacterDetails::MakeInstance();

    TArray<std::tuple<FName, FText, ECategoryPriority::Type>> Calls;
    Mock<IDetailLayoutBuilder> MockLayoutBuilder;
    Mock<IDetailCategoryBuilder> MockDetailCategoryBuilder;
    Fake(Dtor(MockLayoutBuilder));
    When(Method(MockLayoutBuilder, EditCategory))
        .AlwaysDo([&Calls, &MockDetailCategoryBuilder](FName CategoryName, const FText& NewLocalizedDisplayName, ECategoryPriority::Type CategoryType) -> IDetailCategoryBuilder& {
            Calls.Emplace(CategoryName, NewLocalizedDisplayName, CategoryType);
            return MockDetailCategoryBuilder.get();
        });
    TSharedPtr<IDetailLayoutBuilder> LayoutBuilder(&MockLayoutBuilder.get());
    Customization->CustomizeDetails(LayoutBuilder);

    ASSERT_EQUAL(1, Calls.Num());
    CHECK_EQUAL(TEXT("Rendering"), std::get<0>(Calls[0]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[0]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[0]));
    
    return true;
}
#endif