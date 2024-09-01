#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Views/MapKey.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/MapValue.h"

BEGIN_DEFINE_SPEC(FTestMapOperations, "Unit Tests.Ranges.Map",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
 
TMap<int32, int32> DemoMap;
 
END_DEFINE_SPEC(FTestMapOperations);

void FTestMapOperations::Define() {
    Describe("Test Map Operations", [this] {
        BeforeEach([this] {
            DemoMap = {
                {1, 2},
                {2, 4},
                {3, 6}
            };
        });
        
        It("Should be able to extract the keys from a map", [this] {
            // clang-format off
            auto Keys = DemoMap |
                UE::Ranges::MapKey |
                UE::Ranges::ToArray;
            // clang-format on
            UE_ASSERT_EQUAL(3, Keys.Num());
            UE_CHECK_EQUAL(1, Keys[0]);
            UE_CHECK_EQUAL(2, Keys[1]);
            UE_CHECK_EQUAL(3, Keys[2]);

            return true;
        });

        It("SHould be able to extract values from a map", [this] {
            // clang-format off
            auto Keys = DemoMap |
                UE::Ranges::MapValue |
                UE::Ranges::ToArray;
            // clang-format on
            UE_ASSERT_EQUAL(3, Keys.Num());
            UE_CHECK_EQUAL(2, Keys[0]);
            UE_CHECK_EQUAL(4, Keys[1]);
            UE_CHECK_EQUAL(6, Keys[2]);

            return true;
        });
    });
}
