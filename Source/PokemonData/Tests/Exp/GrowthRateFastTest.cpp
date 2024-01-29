#include "Exp/Fast.h"
#include "Misc/AutomationTest.h"

#include <array>

constexpr std::array GFastGrowthTable = {
	-1,
	0, 6, 21, 51, 100, 172, 274, 409, 583, 800,
	1064, 1382, 1757, 2195, 2700, 3276, 3930, 4665, 5487, 6400,
	7408, 8518, 9733, 11059, 12500, 14060, 15746, 17561, 19511, 21600,
	23832, 26214, 28749, 31443, 34300, 37324, 40522, 43897, 47455, 51200,
	55136, 59270, 63605, 68147, 72900, 77868, 83058, 88473, 94119, 100000,
	106120, 112486, 119101, 125971, 133100, 140492, 148154, 156089, 164303, 172800,
	181584, 190662, 200037, 209715, 219700, 229996, 240610, 251545, 262807, 274400,
	286328, 298598, 311213, 324179, 337500, 351180, 365226, 379641, 394431, 409600,
	425152, 441094, 457429, 474163, 491300, 508844, 526802, 545177, 563975, 583200,
	602856, 622950, 643485, 664467, 685900, 707788, 730138, 752953, 776239, 800000
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateFastTest, "Tests.Exp.GrowthRateFastTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateFastTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const {
	OutBeautifiedNames.Add("Tests.Exp.GrowthRateFastTest: Level = 1");
	OutTestCommands.Add("1");

	for (int i = 0; i < 3; i++) {
		auto Fmt = FString::Printf(TEXT("Tests.Exp.GrowthRateFastTest: Level = 2-99 (Sample %d)"), i + 1);
		OutBeautifiedNames.Add(Fmt);
		OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 99)));
	}

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateFastTest: Level = 100");
	OutTestCommands.Add("100");
}

bool GrowthRateFastTest::RunTest(const FString& Parameters) {
	const int32 Level = FCString::Atoi(GetData(Parameters));

	Exp::FFast GrowthRate;
	return TestEqual("The amount of Exp. required to level up should match the expected value!",
	                 GrowthRate.ExpForLevel(Level), GFastGrowthTable[Level]);
}
