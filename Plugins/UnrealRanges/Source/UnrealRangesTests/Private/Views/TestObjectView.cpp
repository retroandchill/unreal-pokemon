#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/ObjectView.h"

BEGIN_DEFINE_SPEC(FTestObjectView, "Unit Tests.Ranges.View",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

END_DEFINE_SPEC(FTestObjectView);

void FTestObjectView::Define() {
    Describe("Test Object View", [this] {
        It("Should be able to iterate through a shared array", [this] {
            auto Result = UE::Ranges::TObjectView() | UE::Ranges::ToArray;
            UE_ASSERT_NOT_EQUAL(0, Result.Num());
            return true;
        });
    });
}