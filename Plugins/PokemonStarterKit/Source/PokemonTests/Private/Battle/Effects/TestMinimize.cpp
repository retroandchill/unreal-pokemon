#include "Asserts.h"
#include "Battle/Effects/TargetEffects/EffectMinimize.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Moves/BattleDamage.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Moves/DefaultMove.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMinimize, "Unit Tests.Battle.Effects.TestMinimize",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMinimize::RunTest(const FString &Parameters) {
    auto Tackle = NewObject<UDefaultMove>()->Initialize(TEXT("TACKLE"));
    auto Stomp = NewObject<UDefaultMove>()->Initialize(TEXT("STOMP"));

    auto TackleBattle = NewObject<UBaseBattleMove>()->Initialize(nullptr, Tackle);
    auto StompBattle = NewObject<UBaseBattleMove>()->Initialize(nullptr, Stomp);
    
    FDamageMultipliers Multipliers;
    auto Minimize = NewObject<UEffectMinimize>();
    IBattlerEffect::Execute_ModifyDamageForTarget(Minimize, Multipliers, nullptr, nullptr, TackleBattle, 0, TEXT(""));
    CHECK_EQUAL(1.f, Multipliers.FinalDamageMultiplier);
    Multipliers.FinalDamageMultiplier = 1.f;
    IBattlerEffect::Execute_ModifyDamageForTarget(Minimize, Multipliers, nullptr, nullptr, StompBattle, 0, TEXT(""));
    CHECK_EQUAL(2.f, Multipliers.FinalDamageMultiplier);
    
    return true;
}