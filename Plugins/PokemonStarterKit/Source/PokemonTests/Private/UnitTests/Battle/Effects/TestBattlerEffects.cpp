#include "Asserts.h"
#include "Battle/Effects/UserEffects/DamageEffects.h"
#include "Battle/Moves/BattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattleMove.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattleEffects, "Unit Tests.Battle.Effects.TestBattlerEffects",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattleEffects::RunTest(const FString &Parameters) {
    CREATE_MOCK(IBattleMove, Move, FMockBattleMove, MockMove);
    EXPECT_CALL(MockMove, IsConfusionAttack).WillOnce(Return(false)).WillRepeatedly(Return(true));

    FDamageMultipliers Multipliers;
    auto ParentalBond = NewObject<UEffectParentalBond>();
    IBattlerEffect::Execute_ModifyDamageForUser(ParentalBond, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    UE_CHECK_EQUAL(0.25f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto MeFirst = NewObject<UEffectMeFirst>();
    IBattlerEffect::Execute_ModifyDamageForUser(MeFirst, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    UE_CHECK_EQUAL(1.5f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto HelpingHand = NewObject<UEffectHelpingHand>();
    IBattlerEffect::Execute_ModifyDamageForUser(HelpingHand, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    UE_CHECK_EQUAL(1.5f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IBattlerEffect::Execute_ModifyDamageForUser(HelpingHand, Multipliers, nullptr, nullptr, Move, 0, TEXT(""));
    UE_CHECK_EQUAL(1.0f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;

    auto Charge = NewObject<UEffectCharge>();
    IBattlerEffect::Execute_ModifyDamageForUser(Charge, Multipliers, nullptr, nullptr, Move, 0, TEXT("ELECTRIC"));
    UE_CHECK_EQUAL(2.0f, Multipliers.PowerMultiplier);
    Multipliers.PowerMultiplier = 1.f;
    IBattlerEffect::Execute_ModifyDamageForUser(Charge, Multipliers, nullptr, nullptr, Move, 0, TEXT("FIRE"));
    UE_CHECK_EQUAL(1.0f, Multipliers.PowerMultiplier);

    return true;
}