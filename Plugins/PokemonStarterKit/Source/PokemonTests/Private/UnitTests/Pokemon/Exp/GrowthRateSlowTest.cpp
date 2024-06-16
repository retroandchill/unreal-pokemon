// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Exp/Slow.h"
#include <array>

constexpr std::array GSlowGrowthTable = {
    -1,     0,      10,      33,      80,      156,     270,     428,     640,     911,    1250,   1663,   2160,
    2746,   3430,   4218,    5120,    6141,    7290,    8573,    10000,   11576,   13310,  15208,  17280,  19531,
    21970,  24603,  27440,   30486,   33750,   37238,   40960,   44921,   49130,   53593,  58320,  63316,  68590,
    74148,  80000,  86151,   92610,   99383,   106480,  113906,  121670,  129778,  138240, 147061, 156250, 165813,
    175760, 186096, 196830,  207968,  219520,  231491,  243890,  256723,  270000,  283726, 297910, 312558, 327680,
    343281, 359370, 375953,  393040,  410636,  428750,  447388,  466560,  486271,  506530, 527343, 548720, 570666,
    593190, 616298, 640000,  664301,  689210,  714733,  740880,  767656,  795070,  823128, 851840, 881211, 911250,
    941963, 973360, 1005446, 1038230, 1071718, 1105920, 1140841, 1176490, 1212873, 1250000};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateSlowTest, "Unit Tests.Core.Exp.GrowthRateSlowTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateSlowTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const {
    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFastTest: Level = 1");
    OutTestCommands.Add("1");

    for (int i = 0; i < 3; i++) {
        auto Fmt =
            FString::Printf(TEXT("Unit Tests.Core.Exp.GrowthRateFastTest: Level = 2-99 (Sample %d)"), i + 1);
        OutBeautifiedNames.Add(Fmt);
        OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 99)));
    }

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFastTest: Level = 100");
    OutTestCommands.Add("100");
}

bool GrowthRateSlowTest::RunTest(const FString &Parameters) {
    const int32 Level = FCString::Atoi(GetData(Parameters));

    Exp::FSlow GrowthRate;
    ASSERT_EQUAL(GSlowGrowthTable[Level], GrowthRate.ExpForLevel(Level));
    return true;
}