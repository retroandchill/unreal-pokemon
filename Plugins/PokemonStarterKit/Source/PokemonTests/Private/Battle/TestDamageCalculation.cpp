#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"

using namespace fakeit;
using namespace accessor;

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
    
    auto Move = NewObject<UBaseBattleMove>();
    Move->Initialize(BaseMove);
    auto [Attack1, Defense1] = Move->GetAttackAndDefense(User, Target);
    CHECK_EQUAL(123, Attack1);
    CHECK_EQUAL(163, Defense1);
    auto [Attack2, Defense2] = Move->GetAttackAndDefense(User, Target);
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
    
    auto Move = NewObject<UBaseBattleMove>();
    Move->Initialize(BaseMove);
    CHECK_EQUAL(33, Move->CalculateBaseDamage(User, Target));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_Full, "Unit Tests.Battle.TestDamageCalculation.FullCalculation",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_Full::RunTest(const FString &Parameters) {
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    
    When(Method(MockUser, GetLevel)).AlwaysReturn(75);
    When(Method(MockUser, GetAttack)).AlwaysReturn(123);
    When(Method(MockUser, GetTypes)).AlwaysReturn({TEXT("ICE")});
    When(Method(MockTarget, GetDefense)).AlwaysReturn(163);
    When(Method(MockTarget, GetTypes)).AlwaysReturn({TEXT("GROUND"), TEXT("DRAGON")});
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Physical);
    When(Method(MockMove, GetBasePower)).AlwaysReturn(65);
    When(Method(MockMove, GetType)).AlwaysReturn(TEXT("ICE"));
    
    auto Move = NewObject<UBaseBattleMove>()->Initialize(BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness] = IBattleMove::Execute_CalculateDamage(Move.GetObject(), User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::SuperEffective, Effeciveness);
        CHECK_TRUE(Damage >= 168);
        // TODO: This is wrong according to Smogon and Bulbapedia, but so far I can't find anything about the math that
        // would modify it down to 196
        CHECK_TRUE(Damage <= 198);
    }
    return true;
}