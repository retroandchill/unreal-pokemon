#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/String/CommonString.h"

BEGIN_DEFINE_SPEC(FTestCommonString, "Unit Tests.Ranges.String",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestCommonString);

void FTestCommonString::Define() {
    Describe("Test Common String Operations", [this] {
        BeforeEach([] { });

        It("FName should store a string as a Common String", [this] {
            FName Sample = "Hello World";
            auto AsCommonString = UE::Ranges::GetString(Sample);
            UE_ASSERT_TRUE(std::holds_alternative<FString>(AsCommonString));
            return true;
        });

        It("An L-Value String should store a string view as a Common String", [this] {
            FString Sample = TEXT("Hello World");
            auto AsCommonString = UE::Ranges::GetString(Sample);
            UE_ASSERT_TRUE(std::holds_alternative<FStringView>(AsCommonString));
            return true;
        });

        It("An R-Value String should store a string value as a Common String", [this] {
            auto AsCommonString = UE::Ranges::GetString(FString(TEXT("Hello World")));
            UE_ASSERT_TRUE(std::holds_alternative<FString>(AsCommonString));
            return true;
        });

        It("An L-Value Text should store a string view as a Common String", [this] {
            auto Sample = FText::FromStringView(TEXT("Hello World"));
            auto AsCommonString = UE::Ranges::GetString(Sample);
            UE_ASSERT_TRUE(std::holds_alternative<FStringView>(AsCommonString));
            return true;
        });

        It("An R-Value Text should store a text value as a Common String", [this] {
            auto AsCommonString = UE::Ranges::GetString(FText::FromStringView(TEXT("Hello World")));
            UE_ASSERT_TRUE(std::holds_alternative<FText>(AsCommonString));
            return true;
        });
        
        It("An string literal should store a string view as a Common String", [this] {
            auto AsCommonString = UE::Ranges::GetString(TEXT("Hello World"));
            UE_ASSERT_TRUE(std::holds_alternative<FStringView>(AsCommonString));
            return true;
        });
    });
}