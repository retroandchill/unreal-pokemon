#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Ranges/Algorithm/GroupingBy.h"
#include "Ranges/Views/MapKey.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/MapValue.h"

BEGIN_DEFINE_SPEC(FTestMapOperations, "Unit Tests.Ranges.View",
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

        It("Should be able to extract values from a map", [this] {
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

        It("Should be able to group a range into a map", [this] {
            std::array Values = {std::make_pair(1, "Number"), std::make_pair(1, "Value"), std::make_pair(2, "Other")};
            auto Maps = Values |
                UE::Ranges::GroupingBy([](const std::pair<int32, const char*>& Pair){ return Pair.first; });
            UE_ASSERT_EQUAL(2, Maps.Num());
            UE_ASSERT_TRUE(Maps.Contains(1));
            UE_ASSERT_EQUAL(2, Maps[1].Num());
            UE_ASSERT_TRUE(Maps.Contains(2));
            UE_ASSERT_EQUAL(1, Maps[1].Num());
            
            return true;
        });
    });
}
