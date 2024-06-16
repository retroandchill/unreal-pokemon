#include "Asserts.h"
#include "Battle/Effects/UserEffects/DamageEffects.h"
#include "Battle/Moves/BattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattleEffects, "Private.UnitTests.Battle.Effects.TestBattlerEffects",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattleEffects::RunTest(const FString &Parameters) {
    auto [Move, MockMove] = UnrealMock::CreateMock<IBattleMove>();
    When(Method(MockMove, IsConfusionAttack)).Return(false).Return(true);

    FDamageMultipliers Multipliers;
    auto ParentalBond = NewObject<UEffectParentalBond>();
    IBattlerEffect::Execute_ModifyDamageForUser(ParentalBond, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    CHECK_EQUAL(0.25f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto MeFirst = NewObject<UEffectMeFirst>();
    IBattlerEffect::Execute_ModifyDamageForUser(MeFirst, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    CHECK_EQUAL(1.5f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto HelpingHand = NewObject<UEffectHelpingHand>();
    IBattlerEffect::Execute_ModifyDamageForUser(HelpingHand, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    CHECK_EQUAL(1.5f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IBattlerEffect::Execute_ModifyDamageForUser(HelpingHand, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    CHECK_EQUAL(1.0f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto Charge = NewObject<UEffectCharge>();
    IBattlerEffect::Execute_ModifyDamageForUser(Charge, Multipliers, nullptr, nullptr, Move, 0, TEXT("ELECTRIC"));
    CHECK_EQUAL(2.0f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IBattlerEffect::Execute_ModifyDamageForUser(Charge, Multipliers, nullptr, nullptr, Move, 0, TEXT("FIRE"));
    CHECK_EQUAL(1.0f, Multipliers.PowerMultiplier);

    return true;
}