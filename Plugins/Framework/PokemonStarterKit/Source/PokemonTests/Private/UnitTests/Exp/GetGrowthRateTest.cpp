// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetGrowthRateTest, "Unit Tests.Core.Exp.GetGrowthRateTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetGrowthRateTest::RunTest(const FString &Parameters) {
    auto &DataSubsystem = FDataManager::GetInstance();

    auto &GrowthRateProxy = DataSubsystem.GetDataTable<FGrowthRateData>();
    auto IdList = GrowthRateProxy.GetTableRowNames();
    UE_ASSERT_NOT_EQUAL(IdList.Num(), 0);
    for (auto ID : IdList) {
        UE_CHECK_TRUE(GrowthRateProxy.IsRowNameValid(ID));

        auto GrowthRate = GrowthRateProxy.GetData(ID);
        if (!UE_CHECK_NOT_NULL(GrowthRate)) {
            continue;
        }

        UE_CHECK_EQUAL(ID, GrowthRate->ID);
    }

    return true;
}