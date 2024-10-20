﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Exp/Erratic.h"
#include <array>

constexpr std::array GErraticGrowthTable = {
    -1,     0,      15,     52,     122,    237,    406,    637,    942,    1326,   1800,   2369,   3041,
    3822,   4719,   5737,   6881,   8155,   9564,   11111,  12800,  14632,  16610,  18737,  21012,  23437,
    26012,  28737,  31610,  34632,  37800,  41111,  44564,  48155,  51881,  55737,  59719,  63822,  68041,
    72369,  76800,  81326,  85942,  90637,  95406,  100237, 105122, 110052, 115015, 120001, 125000, 131324,
    137795, 144410, 151165, 158056, 165079, 172229, 179503, 186894, 194400, 202013, 209728, 217540, 225443,
    233431, 241496, 249633, 257834, 267406, 276458, 286328, 296358, 305767, 316074, 326531, 336255, 346965,
    357812, 367807, 378880, 390077, 400293, 411686, 423190, 433572, 445239, 457001, 467489, 479378, 491346,
    501878, 513934, 526049, 536557, 548720, 560922, 571333, 583539, 591882, 600000};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateErraticTest, "Unit Tests.Core.Exp.GrowthRateErraticTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateErraticTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const {
    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 1");
    OutTestCommands.Add("1");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 2-49");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 49)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 50");
    OutTestCommands.Add("50");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 51");
    OutTestCommands.Add("51");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 52-67");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(52, 67)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 68");
    OutTestCommands.Add("68");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 69");
    OutTestCommands.Add("69");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 70-97");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(70, 97)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 98");
    OutTestCommands.Add("98");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 99");
    OutTestCommands.Add("99");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateErraticTest: Level = 100");
    OutTestCommands.Add("100");
}

bool GrowthRateErraticTest::RunTest(const FString &Parameters) {
    const int32 Level = FCString::Atoi(GetData(Parameters));
    Exp::FErratic GrowthRate;
    UE_ASSERT_EQUAL(GrowthRate.ExpForLevel(Level), GErraticGrowthTable[Level]);
    return true;
}