// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "DataManager.h"
#include "ImportUtils.h"
#include "Misc/AutomationTest.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetMainStatsFromTable, "Unit Tests.PokemonUtilities.TestGetMainStatsFromTable",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetMainStatsFromTable::RunTest(const FString &Parameters) {
    auto StatTable = FDataManager::GetInstance().GetDataTable<FStat>().GetDataTable();
    auto Stats = UImportUtils::GetMainStatsFromTable(StatTable);

    ASSERT_EQUAL(Stats.Num(), 6);
    CHECK_EQUAL(Stats[0].ID, FName(TEXT("HP")));
    CHECK_EQUAL(Stats[1].ID, FName(TEXT("ATTACK")));
    CHECK_EQUAL(Stats[2].ID, FName(TEXT("DEFENSE")));
    CHECK_EQUAL(Stats[3].ID, FName(TEXT("SPECIAL_ATTACK")));
    CHECK_EQUAL(Stats[4].ID, FName(TEXT("SPECIAL_DEFENSE")));
    CHECK_EQUAL(Stats[5].ID, FName(TEXT("SPEED")));

    return true;
}
#endif