#include "Exp/GrowthRate.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateInvalidLevelTest, "Tests.Exp.GrowthRateInvalidLevelTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateInvalidLevelTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const {
	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Medium\"");
	OutTestCommands.Add("Medium");

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Erratic\"");
	OutTestCommands.Add("Erratic");

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Fluctuating\"");
	OutTestCommands.Add("Fluctuating");

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Parabolic\"");
	OutTestCommands.Add("Parabolic");

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Fast\"");
	OutTestCommands.Add("Fast");

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateInvalidLevelTest: Type = \"Slow\"");
	OutTestCommands.Add("Slow");
}

bool GrowthRateInvalidLevelTest::RunTest(const FString& Parameters) {
	auto GrowthRate = Exp::IGrowthRate::GetSubclassRegistry().Construct(TCHAR_TO_UTF8(*Parameters));
	if (!TestNotNull("The specified growth rate doesn't exist!", GrowthRate.Get()))
		return false;

	try {
		GrowthRate->ExpForLevel(-1);
		GrowthRate->ExpForLevel(0);
		return false;
	} catch (const std::invalid_argument&) {
		return true;
	}
}
