// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "ImportUtils.h"
#include "Misc/AutomationTest.h"
#include "Species/Stat.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetMainStatsFromTable, "UnrealPokemon.PokemonUtilities.TestGetMainStatsFromTable",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetMainStatsFromTable::RunTest(const FString &Parameters) {
    auto StatTable = FDataManager::GetInstance().GetDataTable<FStat>().GetDataTable();
    auto Stats = UImportUtils::GetMainStatsFromTable(StatTable);

    bool bPassed = TestEqual(TEXT("Table should have 6 stats!"), 6, Stats.Num());
    bPassed &= TestEqual(TEXT("HP"), FName(TEXT("HP")), Stats[0].ID);
    bPassed &= TestEqual(TEXT("ATTACK"), FName(TEXT("ATTACK")), Stats[1].ID);
    bPassed &= TestEqual(TEXT("DEFENSE"), FName(TEXT("DEFENSE")), Stats[2].ID);
    bPassed &= TestEqual(TEXT("SPECIAL_ATTACK"), FName(TEXT("SPECIAL_ATTACK")), Stats[3].ID);
    bPassed &= TestEqual(TEXT("SPECIAL_DEFENSE"), FName(TEXT("SPECIAL_DEFENSE")), Stats[4].ID);
    bPassed &= TestEqual(TEXT("SPEED"), FName(TEXT("SPEED")), Stats[5].ID);

    return bPassed;
}
