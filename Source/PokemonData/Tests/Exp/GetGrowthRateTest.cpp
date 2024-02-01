#include "DataSubsystem.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetGrowthRateTest, "Tests.Exp.GetGrowthRateTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetGrowthRateTest::RunTest(const FString& Parameters) {
	auto GameInstance = NewObject<UGameInstance>();
	GameInstance->Init();
	auto DataSubsystem = GameInstance->GetSubsystem<UDataSubsystem>();

	bool Passes = true;
	auto IdList = DataSubsystem->GetGrowthRateIds();
	Passes &= TestNotEqual("No Growth Rates Found!", IdList.Num(), 0);
	for (auto ID : IdList) {
		Passes &= TestTrue("Failed to find ID in list!", DataSubsystem->IsGrowthRateIdValid(ID));

		auto GrowthRate = DataSubsystem->GetGrowthRate(ID);
		Passes &= TestEqual("IDs do not match!", GrowthRate.ID, ID);

		auto ImplementationClass = NewObject<UObject>(DataSubsystem, GrowthRate.ImplementationClass);
		auto AsInterface = Cast<IGrowthRate>(ImplementationClass);
		Passes &= TestNotNull("Implementation class does not Implement Growth Rate!", AsInterface);
	}
	
	return Passes;
}
