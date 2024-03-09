// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetGrowthRateTest, "Project.Core.Exp.GetGrowthRateTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetGrowthRateTest::RunTest(const FString& Parameters) {
	auto &DataSubsystem = FDataManager::GetInstance();

	bool Passes = true;
	auto& GrowthRateProxy = DataSubsystem.GetDataTable<FGrowthRateData>();
	auto IdList = GrowthRateProxy.GetTableRowNames();
	Passes &= TestNotEqual("No Growth Rates Found!", IdList.Num(), 0);
	for (auto ID : IdList) {
		Passes &= TestTrue("Failed to find ID in list!", GrowthRateProxy.IsRowNameValid(ID));

		auto GrowthRate = GrowthRateProxy.GetData(ID);
		if (!TestNotNull("Growth rate should not be null!", GrowthRate)) {
			Passes = false;
			continue;
		}

		Passes &= TestEqual("IDs do not match!", GrowthRate->ID, ID);
	}

	return Passes;
}
