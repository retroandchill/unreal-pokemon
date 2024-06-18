#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"
#include "UtilityClasses/BattleActors/CriticalTestMove.h"

using namespace fakeit;
using namespace accessor;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalWeak,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalQuadWeak",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalWeak::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockUser, IsAbilityActive)).AlwaysReturn(false);
    When(Method(MockUser, IsHoldItemActive)).AlwaysReturn(false);
    When(Method(MockUser, GetPokemonLevel)).AlwaysReturn(75);
    When(Method(MockUser, GetAttack)).AlwaysReturn(123);
    When(Method(MockUser, GetTypes)).AlwaysReturn({TEXT("ICE")});
    Fake(Method(MockUser, GetTraitHolders));
    Fake(Method(MockUser, GetAllies));
    When(Method(MockTarget, IsAbilityActive)).AlwaysReturn(false);
    When(Method(MockTarget, IsHoldItemActive)).AlwaysReturn(false);
    When(Method(MockTarget, GetDefense)).AlwaysReturn(163);
    When(Method(MockTarget, GetTypes)).AlwaysReturn({TEXT("GROUND"), TEXT("DRAGON")});
    Fake(Method(MockTarget, GetTraitHolders));
    Fake(Method(MockTarget, GetAllies));
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Physical);
    When(Method(MockMove, GetBasePower)).AlwaysReturn(65);
    When(Method(MockMove, GetType)).AlwaysReturn(TEXT("ICE"));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::SuperEffective, Effeciveness);
        CHECK_TRUE(Damage >= 168);
        CHECK_TRUE(Damage <= 198);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_SpecialResisted,
                                 "Unit Tests.Battle.TestDamageCalculation.SpecialResisted",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_SpecialResisted::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockUser, IsAbilityActive)).AlwaysReturn(false);
    When(Method(MockUser, IsHoldItemActive)).AlwaysReturn(false);
    When(Method(MockUser, GetPokemonLevel)).AlwaysReturn(100);
    When(Method(MockUser, GetSpecialAttack)).AlwaysReturn(359);
    When(Method(MockUser, GetTypes)).AlwaysReturn({TEXT("DARK")});
    Fake(Method(MockUser, GetTraitHolders));
    Fake(Method(MockUser, GetAllies));
    When(Method(MockTarget, IsAbilityActive)).AlwaysReturn(false);
    When(Method(MockTarget, IsHoldItemActive)).AlwaysReturn(false);
    When(Method(MockTarget, GetSpecialDefense)).AlwaysReturn(256);
    When(Method(MockTarget, GetTypes)).AlwaysReturn({TEXT("DRAGON"), TEXT("FAIRY")});
    Fake(Method(MockTarget, GetTraitHolders));
    Fake(Method(MockTarget, GetAllies));
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Special);
    When(Method(MockMove, GetBasePower)).AlwaysReturn(65);
    When(Method(MockMove, GetType)).AlwaysReturn(TEXT("DARK"));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::NotVeryEffective, Effeciveness);
        CHECK_TRUE(Damage >= 49);
        CHECK_TRUE(Damage <= 59);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalNoStab,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalNoStab",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalNoStab::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockUser, IsAbilityActive)).AlwaysReturn(false);
    When(Method(MockUser, IsHoldItemActive)).AlwaysReturn(false);
    When(Method(MockUser, GetPokemonLevel)).AlwaysReturn(100);
    When(Method(MockUser, GetAttack)).AlwaysReturn(319);
    When(Method(MockUser, GetTypes)).AlwaysReturn({TEXT("FIGHTING"), TEXT("STEEL")});
    Fake(Method(MockUser, GetTraitHolders));
    Fake(Method(MockUser, GetAllies));
    When(Method(MockTarget, GetDefense)).AlwaysReturn(277);
    When(Method(MockTarget, GetTypes)).AlwaysReturn({TEXT("WATER")});
    Fake(Method(MockTarget, GetTraitHolders));
    Fake(Method(MockTarget, GetAllies));
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Physical);
    When(Method(MockMove, GetBasePower)).AlwaysReturn(80);
    When(Method(MockMove, GetType)).AlwaysReturn(TEXT("NORMAL"));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::Normal, Effeciveness);
        CHECK_TRUE(Damage >= 67);
        CHECK_TRUE(Damage <= 79);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_SpecialImmune,
                                 "Unit Tests.Battle.TestDamageCalculation.SpecialImmune",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_SpecialImmune::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockTarget, GetTypes)).AlwaysReturn({TEXT("GHOST")});
    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Special);
    When(Method(MockMove, GetType)).AlwaysReturn(TEXT("NORMAL"));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::NoEffect, Effeciveness);
        CHECK_EQUAL(0, Damage);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_StatusMove,
                                 "Unit Tests.Battle.TestDamageCalculation.StatusMove",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_StatusMove::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();

    When(Method(MockMove, GetDamageCategory)).AlwaysReturn(EMoveDamageCategory::Status);

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        CHECK_EQUAL(EDamageEffectiveness::NonDamaging, Effeciveness);
        CHECK_EQUAL(0, Damage);
    }
    return true;
}