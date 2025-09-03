﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Exp/Medium.h"
#include <array>

constexpr std::array GMediumGrowthTable = {
    -1,     0,      8,      27,     64,     125,    216,    343,    512,    729,    1000,   1331,   1728,
    2197,   2744,   3375,   4096,   4913,   5832,   6859,   8000,   9261,   10648,  12167,  13824,  15625,
    17576,  19683,  21952,  24389,  27000,  29791,  32768,  35937,  39304,  42875,  46656,  50653,  54872,
    59319,  64000,  68921,  74088,  79507,  85184,  91125,  97336,  103823, 110592, 117649, 125000, 132651,
    140608, 148877, 157464, 166375, 175616, 185193, 195112, 205379, 216000, 226981, 238328, 250047, 262144,
    274625, 287496, 300763, 314432, 328509, 343000, 357911, 373248, 389017, 405224, 421875, 438976, 456533,
    474552, 493039, 512000, 531441, 551368, 571787, 592704, 614125, 636056, 658503, 681472, 704969, 729000,
    753571, 778688, 804357, 830584, 857375, 884736, 912673, 941192, 970299, 1000000};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateMediumTest, "Unit Tests.Core.Exp.GrowthRateMediumTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateMediumTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const {
    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateMediumTest: Level = 1");
    OutTestCommands.Add("1");

    for (int i = 0; i < 3; i++) {
        auto Fmt = FString::Printf(TEXT("Unit Tests.Core.Exp.GrowthRateMediumTest: Level = 2-99 (Sample %d)"), i + 1);
        OutBeautifiedNames.Add(Fmt);
        OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 99)));
    }

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateMediumTest: Level = 100");
    OutTestCommands.Add("100");
}

bool GrowthRateMediumTest::RunTest(const FString &Parameters) {
    const int32 Level = FCString::Atoi(GetData(Parameters));

    Exp::FMedium GrowthRate;
    UE_ASSERT_EQUAL(GMediumGrowthTable[Level], GrowthRate.ExpForLevel(Level));
    return true;
}