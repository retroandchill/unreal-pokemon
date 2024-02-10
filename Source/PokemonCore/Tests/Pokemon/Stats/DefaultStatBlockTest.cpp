#include "Misc/AutomationTest.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultStatBlockTest, "Tests.DefaultStatBlockTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultStatBlockTest::RunTest(const FString& Parameters) {
	TMap<FName, int32> BaseStats = {
		{"HP", 108},
		{"ATTACK", 130},
		{"DEFENSE", 95},
		{"SPECIAL_ATTACK", 80},
		{"SPECIAL_DEFENSE", 85},
		{"SPEED", 102}
	};

	TMap<FName, int32> IVs = {
		{"HP", 24},
		{"ATTACK", 12},
		{"DEFENSE", 30},
		{"SPECIAL_ATTACK", 16},
		{"SPECIAL_DEFENSE", 23},
		{"SPEED", 5}
	};

	TMap<FName, int32> EVs = {
		{"HP", 74},
		{"ATTACK", 190},
		{"DEFENSE", 91},
		{"SPECIAL_ATTACK", 48},
		{"SPECIAL_DEFENSE", 84},
		{"SPEED", 23}
	};

	FDefaultStatBlock Block("Slow", 78, IVs, EVs, "ADAMANT");
	Block.CalculateStats(BaseStats);

	int32 HP = Block.GetStat("HP").GetStatValue();
	bool Passed = TestEqual("HP", HP, 289);

	int32 Atk = Block.GetStat("ATTACK").GetStatValue();
	Passed &= TestEqual("Attack", Atk, 278);

	int32 Def = Block.GetStat("DEFENSE").GetStatValue();
	Passed &= TestEqual("Defense", Def, 193);

	int32 SpA = Block.GetStat("SPECIAL_ATTACK").GetStatValue();
	Passed &= TestEqual("Sp. Atk", SpA, 135);

	int32 SpD = Block.GetStat("SPECIAL_DEFENSE").GetStatValue();
	Passed &= TestEqual("Sp. Def", SpD, 171);

	int32 Spe = Block.GetStat("SPEED").GetStatValue();
	Passed &= TestEqual("Speed", Spe, 171);

	int32 Exp = Block.GetExp();
	Passed &= TestEqual("Exp.", Exp, 593190);

	int32 NextLevel = Block.GetExpForNextLevel();
	Passed &= TestEqual("Next Level Exp.", NextLevel, 616298);
	
	return Passed;
}
