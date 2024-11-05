#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/ToSet.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/SharedArrayView.h"

BEGIN_DEFINE_SPEC(FTestSharedArrayView, "Unit Tests.Ranges.View",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestSharedArrayView);

void FTestSharedArrayView::Define() {
    Describe("Test Shared Array View", [this] {
        It("Should be able to iterate through a shared array", [this] {
            // clang-format off
            auto Values = UE::Ranges::TSharedArrayView<int32>({1, 2, 3, 4, 5}) |
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