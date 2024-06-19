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
#include "Mocks/MockMove.h"

using namespace accessor;
using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalWeak,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalQuadWeak",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalWeak::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove, FMockMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    ON_CALL(MockUser, IsAbilityActive).WillByDefault(Return(false));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(75));
    ON_CALL(MockUser, GetAttack).WillByDefault(Return(123));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("ICE")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    ON_CALL(MockTarget, IsAbilityActive).WillByDefault(Return(false));
    ON_CALL(MockTarget, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockTarget, GetDefense).WillByDefault(Return(163));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("GROUND"), TEXT("DRAGON")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Physical));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(65));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("ICE")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        UE_CHECK_EQUAL(EDamageEffectiveness::SuperEffective, Effeciveness);
        UE_CHECK_TRUE(Damage >= 168);
        UE_CHECK_TRUE(Damage <= 198);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_SpecialResisted,
                                 "Unit Tests.Battle.TestDamageCalculation.SpecialResisted",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_SpecialResisted::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove, FMockMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    ON_CALL(MockUser, IsAbilityActive).WillByDefault(Return(false));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(100));
    ON_CALL(MockUser, GetSpecialAttack).WillByDefault(Return(359));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("DARK")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    ON_CALL(MockTarget, IsAbilityActive).WillByDefault(Return(false));
    ON_CALL(MockTarget, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockTarget, GetSpecialDefense).WillByDefault(Return(256));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("DRAGON"), TEXT("FAIRY")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Special));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(65));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("DARK")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        UE_CHECK_EQUAL(EDamageEffectiveness::NotVeryEffective, Effeciveness);
        UE_CHECK_TRUE(Damage >= 49);
        UE_CHECK_TRUE(Damage <= 59);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalNoStab,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalNoStab",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalNoStab::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove, FMockMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    ON_CALL(MockUser, IsAbilityActive).WillByDefault(Return(false));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(100));
    ON_CALL(MockUser, GetAttack).WillByDefault(Return(319));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("FIGHTING"), TEXT("STEEL")}));
    //Fake(Method(MockUser, GetTraitHolders));
    //Fake(Method(MockUser, GetAllies));
    ON_CALL(MockTarget, GetDefense).WillByDefault(Return(277));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("WATER")}));
    //Fake(Method(MockTarget, GetTraitHolders));
    //Fake(Method(MockTarget, GetAllies));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Physical));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(80));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("NORMAL")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        UE_CHECK_EQUAL(EDamageEffectiveness::Normal, Effeciveness);
        UE_CHECK_TRUE(Damage >= 67);
        UE_CHECK_TRUE(Damage <= 79);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_SpecialImmune,
                                 "Unit Tests.Battle.TestDamageCalculation.SpecialImmune",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_SpecialImmune::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove, FMockMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("GHOST")}));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Special));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("NORMAL")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        UE_CHECK_EQUAL(EDamageEffectiveness::NoEffect, Effeciveness);
        UE_CHECK_EQUAL(0, Damage);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_StatusMove,
                                 "Unit Tests.Battle.TestDamageCalculation.StatusMove",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_StatusMove::RunTest(const FString &Parameters) {
    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [User, MockUser] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    auto [BaseMove, MockMove] = UnrealMock::CreateMock<IMove, FMockMove>();
    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();

    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Status));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    for (int i = 0; i < 100; i++) {
        auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
        UE_CHECK_EQUAL(EDamageEffectiveness::NonDamaging, Effeciveness);
        UE_CHECK_EQUAL(0, Damage);
    }
    return true;
}