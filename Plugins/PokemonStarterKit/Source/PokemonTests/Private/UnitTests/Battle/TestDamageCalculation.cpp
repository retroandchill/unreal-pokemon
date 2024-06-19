#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "External/accessor.hpp"
#include "External/fakeit.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"
#include "UtilityClasses/BattleActors/CriticalTestMove.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattler.h"

using namespace accessor;
using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalWeak,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalQuadWeak",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalWeak::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    EXPECT_CALL(MockUser, IsAbilityActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, IsHoldItemActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, GetPokemonLevel).WillRepeatedly(Return(75));
    EXPECT_CALL(MockUser, GetAttack).WillRepeatedly(Return(123));
    EXPECT_CALL(MockUser, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("ICE")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    EXPECT_CALL(MockTarget, IsAbilityActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockTarget, IsHoldItemActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockTarget, GetDefense).WillRepeatedly(Return(163));
    EXPECT_CALL(MockTarget, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("GROUND"), TEXT("DRAGON")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    EXPECT_CALL(MockMove, GetDamageCategory).WillRepeatedly(Return(EMoveDamageCategory::Physical));
    EXPECT_CALL(MockMove, GetBasePower).WillRepeatedly(Return(65));
    EXPECT_CALL(MockMove, GetType).WillRepeatedly(Return(TEXT("ICE")));

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
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    EXPECT_CALL(MockUser, IsAbilityActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, IsHoldItemActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, GetPokemonLevel).WillRepeatedly(Return(100));
    EXPECT_CALL(MockUser, GetSpecialAttack).WillRepeatedly(Return(359));
    EXPECT_CALL(MockUser, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("DARK")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    EXPECT_CALL(MockTarget, IsAbilityActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockTarget, IsHoldItemActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockTarget, GetSpecialDefense).WillRepeatedly(Return(256));
    EXPECT_CALL(MockTarget, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("DRAGON"), TEXT("FAIRY")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    EXPECT_CALL(MockMove, GetDamageCategory).WillRepeatedly(Return(EMoveDamageCategory::Special));
    EXPECT_CALL(MockMove, GetBasePower).WillRepeatedly(Return(65));
    EXPECT_CALL(MockMove, GetType).WillRepeatedly(Return(TEXT("DARK")));

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
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    EXPECT_CALL(MockUser, IsAbilityActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, IsHoldItemActive).WillRepeatedly(Return(false));
    EXPECT_CALL(MockUser, GetPokemonLevel).WillRepeatedly(Return(100));
    EXPECT_CALL(MockUser, GetAttack).WillRepeatedly(Return(319));
    EXPECT_CALL(MockUser, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("FIGHTING"), TEXT("STEEL")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    EXPECT_CALL(MockTarget, GetDefense).WillRepeatedly(Return(277));
    EXPECT_CALL(MockTarget, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("WATER")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    EXPECT_CALL(MockMove, GetDamageCategory).WillRepeatedly(Return(EMoveDamageCategory::Physical));
    EXPECT_CALL(MockMove, GetBasePower).WillRepeatedly(Return(80));
    EXPECT_CALL(MockMove, GetType).WillRepeatedly(Return(TEXT("NORMAL")));

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
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    EXPECT_CALL(MockTarget, GetTypes).WillRepeatedly(Return<TArray<FName>>({TEXT("GHOST")}));
    EXPECT_CALL(MockMove, GetDamageCategory).WillRepeatedly(Return(EMoveDamageCategory::Special));
    EXPECT_CALL(MockMove, GetType).WillRepeatedly(Return(TEXT("NORMAL")));

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
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    EXPECT_CALL(MockMove, GetDamageCategory).WillRepeatedly(Return(EMoveDamageCategory::Status));

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