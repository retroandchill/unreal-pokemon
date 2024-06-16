#include "Asserts.h"
#include "Battle/Effects/FieldEffects/SportMoves.h"
#include "Battle/Moves/BattleDamage.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestSportEffects, "Private.UnitTests.Battle.Effects.TestSportEffects",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestSportEffects::RunTest(const FString &Parameters) {
    FDamageMultipliers Multipliers;
    auto MudSport = NewObject<UEffectMudSport>();
    IFieldEffect::Execute_ModifyDamage(MudSport, Multipliers, nullptr, nullptr, nullptr, 0, TEXT("ELECTRIC"));
    CHECK_EQUAL(0.33333f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IFieldEffect::Execute_ModifyDamage(MudSport, Multipliers, nullptr, nullptr, nullptr, 0, TEXT("WATER"));
    CHECK_EQUAL(1.f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto WaterSport = NewObject<UEffectWaterSport>();
    IFieldEffect::Execute_ModifyDamage(WaterSport, Multipliers, nullptr, nullptr, nullptr, 0, TEXT("FIRE"));
    CHECK_EQUAL(0.33333f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IFieldEffect::Execute_ModifyDamage(WaterSport, Multipliers, nullptr, nullptr, nullptr, 0, TEXT("GRASS"));
    CHECK_EQUAL(1.f, Multipliers.PowerMultiplier);

    return true;
}