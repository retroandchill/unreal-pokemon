#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/ToSet.h"
#include "Ranges/Views/Map.h"

BEGIN_DEFINE_SPEC(FTestSetOperations, "Unit Tests.Ranges.View",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
 
TArray<int32> DemoArray;
 
END_DEFINE_SPEC(FTestSetOperations);

void FTestSetOperations::Define() {
    Describe("Test Set Operations", [this] {
        BeforeEach([this] {
            DemoArray = {1, 2, 3, 4, 5};
        });
        
        It("Should be able to convert a view into a set", [this] {
            // clang-format off
            auto Values = DemoArray |
                UE::Ranges::Map([](int32 I) { return I * 2; }) |
                UE::Ranges::ToSet;
            // clang-format on
            UE_ASSERT_EQUAL(5, Values.Num());
            UE_CHECK_TRUE(Values.Contains(2));
            UE_CHECK_TRUE(Values.Contains(4));
            UE_CHECK_TRUE(Values.Contains(6));
            UE_CHECK_TRUE(Values.Contains(8));
            UE_CHECK_TRUE(Values.Contains(10));

            return true;
        });
    });
}