// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Exp/Fluctuating.h"
#include <array>

constexpr std::array GFluctuatingGrowthTable = {
    -1,      0,       4,       13,      32,      65,      112,     178,     276,     393,    540,     745,     967,
    1230,    1591,    1957,    2457,    3046,    3732,    4526,    5440,    6482,    7666,   9003,    10506,   12187,
    14060,   16140,   18439,   20974,   23760,   26811,   30146,   33780,   37731,   42017,  46656,   50653,   55969,
    60505,   66560,   71677,   78533,   84277,   91998,   98415,   107069,  114205,  123863, 131766,  142500,  151222,
    163105,  172697,  185807,  196322,  210739,  222231,  238036,  250562,  267840,  281456, 300293,  315059,  335544,
    351520,  373744,  390991,  415050,  433631,  459620,  479600,  507617,  529063,  559209, 582187,  614566,  639146,
    673863,  700115,  737280,  765275,  804997,  834809,  877201,  908905,  954084,  987754, 1035837, 1071552, 1122660,
    1160499, 1214753, 1254796, 1312322, 1354652, 1415577, 1460276, 1524731, 1571884, 1640000};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GrowthRateFluctuatingTest, "Unit Tests.Core.Exp.GrowthRateFluctuatingTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void GrowthRateFluctuatingTest::GetTests(TArray<FString> &OutBeautifiedNames, TArray<FString> &OutTestCommands) const {
    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 1");
    OutTestCommands.Add("1");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 2-14");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(2, 14)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 15");
    OutTestCommands.Add("15");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 16");
    OutTestCommands.Add("15");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 17-34");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(17, 34)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 35");
    OutTestCommands.Add("35");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 36");
    OutTestCommands.Add("36");

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 37-99");
    OutTestCommands.Add(FString::FromInt(FMath::RandRange(37, 99)));

    OutBeautifiedNames.Add("Unit Tests.Core.Exp.GrowthRateFluctuatingTest: Level = 100");
    OutTestCommands.Add("100");
}

bool GrowthRateFluctuatingTest::RunTest(const FString &Parameters) {
    const int32 Level = FCString::Atoi(GetData(Parameters));

    Exp::FFluctuating GrowthRate;
    UE_ASSERT_EQUAL(GFluctuatingGrowthTable[Level], GrowthRate.ExpForLevel(Level));
    return true;
}