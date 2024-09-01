#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/Reduce.h"
#include "Ranges/Views/Map.h"

BEGIN_DEFINE_SPEC(FTestReduce, "Unit Tests.Ranges.Algorithm",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
 
TArray<int32> DemoArray;
 
END_DEFINE_SPEC(FTestReduce);

void FTestReduce::Define() {
    Describe("Test Reduce Operation", [this] {
        BeforeEach([this] {
            DemoArray = {1, 2, 3, 4, 5};
        });
        
        It("Should be able to reduce a range", [this] {
            // clang-format off
            auto Value = DemoArray |
                UE::Ranges::Reduce(0, [](int32 A, int32 B) { return A + B; });
            // clang-format on
            UE_ASSERT_EQUAL(15, Value);

            return true;
        });

        It("Should be able to reduce a range of a different type", [this] {
            // clang-format off
            auto Value = DemoArray |
                UE::Ranges::Reduce(0.0, [](int32 A, int32 B) { return A + B; });
            // clang-format on
            UE_ASSERT_EQUAL(15.0, Value);

            return true;
        });
    });
}