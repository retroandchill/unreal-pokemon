#include "TestAdapter.h"
#include "Misc/AutomationTest.h"
#include "RetroLib.h"


BEGIN_DEFINE_SPEC(FTestCommonString, "Unit Tests.Ranges.String",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestCommonString);

void FTestCommonString::Define() {
    Describe("Test Common String Operations", [this] {
        BeforeEach([] {});

        It("FName should store a string as a Common String", [this] {
            FName Sample = "Hello World";
            auto AsCommonString = Retro::GetString(Sample);
            REQUIRE(std::holds_alternative<FString>(AsCommonString));
        });

        It("An L-Value String should store a string view as a Common String", [this] {
            FString Sample = TEXT("Hello World");
            auto AsCommonString = Retro::GetString(Sample);
            REQUIRE(std::holds_alternative<FStringView>(AsCommonString));
        });

        It("An R-Value String should store a string value as a Common String", [this] {
            auto AsCommonString = Retro::GetString(FString(TEXT("Hello World")));
            REQUIRE(std::holds_alternative<FString>(AsCommonString));
        });

        It("An L-Value Text should store a string view as a Common String", [this] {
            auto Sample = FText::FromStringView(TEXT("Hello World"));
            auto AsCommonString = Retro::GetString(Sample);
            REQUIRE(std::holds_alternative<FStringView>(AsCommonString));
        });

        It("An R-Value Text should store a text value as a Common String", [this] {
            auto AsCommonString = Retro::GetString(FText::FromStringView(TEXT("Hello World")));
            REQUIRE(std::holds_alternative<FText>(AsCommonString));
        });

        It("An string literal should store a string view as a Common String", [this] {
            auto AsCommonString = Retro::GetString(TEXT("Hello World"));
            REQUIRE(std::holds_alternative<FStringView>(AsCommonString));
        });
    });
}