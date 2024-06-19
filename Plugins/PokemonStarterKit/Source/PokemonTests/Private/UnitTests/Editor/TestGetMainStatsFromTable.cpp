// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "DataManager.h"
#include "Misc/AutomationTest.h"
#include "PBS/ImportUtils.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetMainStatsFromTable,
                                 "Unit Tests.PokemonUtilities.TestGetMainStatsFromTable",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetMainStatsFromTable::RunTest(const FString &Parameters) {
    auto StatTable = FDataManager::GetInstance().GetDataTable<FStat>().GetDataTable();
    auto Stats = UImportUtils::GetMainStatsFromTable(StatTable);

    UE_ASSERT_EQUAL(Stats.Num(), 6);
    UE_CHECK_EQUAL(Stats[0].ID, FName(TEXT("HP")));
    UE_CHECK_EQUAL(Stats[1].ID, FName(TEXT("ATTACK")));
    UE_CHECK_EQUAL(Stats[2].ID, FName(TEXT("DEFENSE")));
    UE_CHECK_EQUAL(Stats[3].ID, FName(TEXT("SPECIAL_ATTACK")));
    UE_CHECK_EQUAL(Stats[4].ID, FName(TEXT("SPECIAL_DEFENSE")));
    UE_CHECK_EQUAL(Stats[5].ID, FName(TEXT("SPEED")));

    return true;
}