﻿#include "Exp/Parabolic.h"
#include "Misc/AutomationTest.h"

#include <array>

constexpr std::array GParabolicGrowthTable = {
	-1,
	0, 9, 57, 96, 135, 179, 236, 314, 419, 560,
	742, 973, 1261, 1612, 2035, 2535, 3120, 3798, 4575, 5460,
	6458, 7577, 8825, 10208, 11735, 13411, 15244, 17242, 19411, 21760,
	24294, 27021, 29949, 33084, 36435, 40007, 43808, 47846, 52127, 56660,
	61450, 66505, 71833, 77440, 83335, 89523, 96012, 102810, 109923, 117360,
	125126, 133229, 141677, 150476, 159635, 169159, 179056, 189334, 199999, 211060,
	222522, 234393, 246681, 259392, 272535, 286115, 300140, 314618, 329555, 344960,
	360838, 377197, 394045, 411388, 429235, 447591, 466464, 485862, 505791, 526260,
	547274, 568841, 590969, 613664, 636935, 660787, 685228, 710266, 735907, 762160,
	789030, 816525, 844653, 873420, 902835, 932903, 963632, 995030, 1027103, 1059860
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateParabolicTest, "Tests.Exp.GrowthRateParabolicTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateParabolicTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const {
	OutBeautifiedNames.Add("Tests.Exp.GrowthRateParabolicTest: Level = 1");
	OutTestCommands.Add("1");

	for (int i = 0; i < 3; i++) {
		auto Fmt = FString::Printf(TEXT("Tests.Exp.GrowthRateParabolicTest: Level = 2-99 (Sample %d)"), i + 1);
		OutBeautifiedNames.Add(Fmt);
		OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 99)));
	}

	OutBeautifiedNames.Add("Tests.Exp.GrowthRateParabolicTest: Level = 100");
	OutTestCommands.Add("100");
}

bool GrowthRateParabolicTest::RunTest(const FString& Parameters) {
	const int32 Level = FCString::Atoi(GetData(Parameters));

	UParabolic GrowthRate;
	return TestEqual("The amount of Exp. required to level up should match the expected value!",
	                 GrowthRate.ExpForLevel(Level), GParabolicGrowthTable[Level]);
}
