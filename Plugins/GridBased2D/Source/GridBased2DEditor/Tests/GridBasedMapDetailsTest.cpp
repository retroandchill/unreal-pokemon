#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Details/GridBasedMapDetails.h"
#include "Asserts.h"
#include "DetailLayoutBuilder.h"
#include "Misc/AutomationTest.h"
#include "External/fakeit.hpp"
#include "DetailCategoryBuilder.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridBasedMapDetailsTest, "Tests.GridBasedMapDetailsTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GridBasedMapDetailsTest::RunTest(const FString &Parameters) {
    auto Customization = FGridBasedMapDetails::MakeInstance();

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

    ASSERT_EQUAL(5, Calls.Num());
    
    CHECK_EQUAL(TEXT("Tile Map"), std::get<0>(Calls[0]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[0]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[0]));
    
    CHECK_EQUAL(TEXT("Audio"), std::get<0>(Calls[1]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[1]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[1]));

    CHECK_EQUAL(TEXT("Tiles"), std::get<0>(Calls[2]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[2]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[2]));

    CHECK_EQUAL(TEXT("Z-Sorting"), std::get<0>(Calls[3]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[3]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[3]));

    CHECK_EQUAL(TEXT("Connections"), std::get<0>(Calls[4]).ToString());
    CHECK_EQUAL(TEXT(""), std::get<1>(Calls[4]).ToString());
    CHECK_EQUAL(ECategoryPriority::Important, std::get<2>(Calls[4]));
    
    return true;
}
#endif