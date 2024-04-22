// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS

#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"
#include "Assertions/Asserts.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetGrowthRateTest, "UnrealPokemon.Core.Exp.GetGrowthRateTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetGrowthRateTest::RunTest(const FString &Parameters) {
    auto &DataSubsystem = FDataManager::GetInstance();
    
    auto &GrowthRateProxy = DataSubsystem.GetDataTable<FGrowthRateData>();
    auto IdList = GrowthRateProxy.GetTableRowNames();
    ASSERT_NOT_EQUAL(IdList.Num(), 0);
    for (auto ID : IdList) {
        CHECK_TRUE(GrowthRateProxy.IsRowNameValid(ID));

        auto GrowthRate = GrowthRateProxy.GetData(ID);
        if (!CHECK_NOT_NULL(GrowthRate)) {
            continue;
        }

        CHECK_EQUAL(ID, GrowthRate->ID);
    }

    return true;
}
#endif