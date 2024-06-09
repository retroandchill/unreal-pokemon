#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Abilities/Effects/AuraAbilities.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAuraMoves, "Unit Tests.Battle.Abilities.TestAuraAbilities",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAuraMoves::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto Move = NewObject<UBaseBattleMove>()->Initialize(Battle, nullptr);


    bool bAuraBreak = false;
    When(Method(MockBattle, FindGlobalAbility)).AlwaysDo([&bAuraBreak](FName Ability) {
        return Ability == TEXT("AURABREAK") && bAuraBreak;
    });

    FDamageMultipliers Multipliers;
    auto DarkAura = NewObject<UDarkAura>();
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(DarkAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("DARK"));
    CHECK_EQUAL(4.f / 3.f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(DarkAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("PSYCHIC"));
    CHECK_EQUAL(1.f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    bAuraBreak = true;
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(DarkAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("DARK"));
    CHECK_EQUAL(0.75f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    bAuraBreak = false;

    auto FairyAura = NewObject<UFairyAura>();
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(FairyAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("FAIRY"));
    CHECK_EQUAL(4.f / 3.f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(FairyAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("FIRE"));
    CHECK_EQUAL(1.f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    bAuraBreak = true;
    IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(FairyAura, Multipliers, nullptr, nullptr, Move, 0, TEXT("FAIRY"));
    CHECK_EQUAL(0.75f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    bAuraBreak = false;
    
    return true;
}