#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_AtkDef, "Unit Tests.Battle.TestDamageCalculation.AttackAndDefense",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_AtkDef::RunTest(const FString &Parameters) {
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    
    When(Method(MockUser, GetAttack)).AlwaysReturn(123);
    When(Method(MockUser, GetSpecialAttack)).AlwaysReturn(110);
    When(Method(MockTarget, GetDefense)).AlwaysReturn(163);
    When(Method(MockTarget, GetSpecialDefense)).AlwaysReturn(170);
    When(Method(MockMove, GetDamageCategory)).Return(EMoveDamageCategory::Physical).Return(EMoveDamageCategory::Special);
    
    auto Move = NewObject<UBaseBattleMove>()->Initialize(BaseMove);
    auto [Attack1, Defense1] = IBattleMove::Execute_GetAttackAndDefense(Move.GetObject(), User, Target);
    CHECK_EQUAL(123, Attack1);
    CHECK_EQUAL(163, Defense1);
    auto [Attack2, Defense2] = IBattleMove::Execute_GetAttackAndDefense(Move.GetObject(), User, Target);
    CHECK_EQUAL(110, Attack2);
    CHECK_EQUAL(170, Defense2);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_BaseDamage, "Unit Tests.Battle.TestDamageCalculation.BaseDamage",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_BaseDamage::RunTest(const FString &Parameters) {
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    
    When(Method(MockUser, GetLevel)).AlwaysReturn(75);
    When(Method(MockUser, GetAttack)).AlwaysReturn(123);
    When(Method(MockTarget, GetDefense)).AlwaysReturn(163);
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Physical);
    When(Method(MockMove, GetBasePower)).AlwaysReturn(65);
    
    auto Move = NewObject<UBaseBattleMove>()->Initialize(BaseMove);
    CHECK_EQUAL(33, IBattleMove::Execute_CalculateBaseDamage(Move.GetObject(), User, Target));
    return true;
}