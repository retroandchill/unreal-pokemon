﻿#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Moves/Move.h"
#include "UtilityClasses/BattleActors/CriticalTestMove.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockMove.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace testing;

static UBattlerAbilityComponent* AddComponentToBattler(const TScriptInterface<IBattler>& Battler) {
    auto Actor = Cast<AActor>(Battler.GetObject());
    auto Component = static_cast<UBattlerAbilityComponent*>(Actor->AddComponentByClass(UBattlerAbilityComponent::StaticClass(),
        false, FTransform(), false));
    Component->SetOwnerActor(Actor);
    Component->SetAvatarActor(Actor);
    Component->AbilityActorInfo->OwnerActor = Actor;
    Component->AbilityActorInfo->AvatarActor = Actor;
    return Component;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysicalNoStab,
                                 "Unit Tests.Battle.TestDamageCalculation.PhysicalNoStab",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysicalNoStab::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    
    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IMove, BaseMove, FMockMove, MockMove);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Target, FMockBattler, MockTarget);
    auto FakeAbilityComponent = AddComponentToBattler(User);
    AddComponentToBattler(Target);
    
    ON_CALL(MockUser, GetAbilityComponent).WillByDefault(Return(FakeAbilityComponent));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(100));
    ON_CALL(MockUser, GetAttack).WillByDefault(Return(FMainBattleStat{319, 0}));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("FIGHTING"), TEXT("STEEL")}));
    ON_CALL(MockTarget, GetDefense).WillByDefault(Return(FMainBattleStat{277, 0}));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("WATER")}));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Physical));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(80));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("NORMAL")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
    UE_CHECK_EQUAL(EDamageEffectiveness::Normal, Effeciveness);
    UE_CHECK_EQUAL(79, Damage);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_SpecialNoStab,
                                 "Unit Tests.Battle.TestDamageCalculation.SpecialNoStab",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_SpecialNoStab::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    
    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IMove, BaseMove, FMockMove, MockMove);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Target, FMockBattler, MockTarget);
    auto FakeAbilityComponent = AddComponentToBattler(User);
    AddComponentToBattler(Target);
    
    ON_CALL(MockUser, GetAbilityComponent).WillByDefault(Return(FakeAbilityComponent));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(100));
    ON_CALL(MockUser, GetSpecialAttack).WillByDefault(Return(FMainBattleStat{319, 0}));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("FIGHTING"), TEXT("STEEL")}));
    ON_CALL(MockTarget, GetSpecialDefense).WillByDefault(Return(FMainBattleStat{277, 0}));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("WATER")}));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Special));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(80));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("NORMAL")));

    auto Move = NewObject<UCriticalTestMove>();
    Move->CriticalOverride = ECriticalOverride::Never;
    Move->Initialize(Battle, BaseMove);
    auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
    UE_CHECK_EQUAL(EDamageEffectiveness::Normal, Effeciveness);
    UE_CHECK_EQUAL(79, Damage);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_StatusMove,
                                 "Unit Tests.Battle.TestDamageCalculation.StatusMove",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_StatusMove::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    
    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IMove, BaseMove, FMockMove, MockMove);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Target, FMockBattler, MockTarget);
    auto FakeAbilityComponent = AddComponentToBattler(User);
    AddComponentToBattler(Target);
    
    ON_CALL(MockUser, GetAbilityComponent).WillByDefault(Return(FakeAbilityComponent));
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestCriticalHits, "Unit Tests.Battle.TestDamageCalculation.CriticalHit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestCriticalHits::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    
    // Make the test pass by returning true, or fail by returning false.
    FMath::RandInit(151566);

    // Roll 23 random numbers so that the next one %25 is 0
    for (int32 i = 0; i < 30; i++) {
        int32 Unused = FMath::Rand() % 24;
    }

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, User, FMockBattler, MockUser);
    CREATE_MOCK(IMove, BaseMove, FMockMove, MockMove);
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Target, FMockBattler, MockTarget);
    auto FakeAbilityComponent = AddComponentToBattler(User);
    AddComponentToBattler(Target);
    
    ON_CALL(MockUser, GetAbilityComponent).WillByDefault(Return(FakeAbilityComponent));
    ON_CALL(MockUser, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockUser, GetPokemonLevel).WillByDefault(Return(75));
    ON_CALL(MockUser, GetAttack).WillByDefault(Return(FMainBattleStat{123, -3}));
    ON_CALL(MockUser, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("ICE")}));
    ON_CALL(MockTarget, IsHoldItemActive).WillByDefault(Return(false));
    ON_CALL(MockTarget, GetDefense).WillByDefault(Return(FMainBattleStat{163, 4}));
    ON_CALL(MockTarget, GetTypes).WillByDefault(Return<TArray<FName>>({TEXT("GROUND"), TEXT("DRAGON")}));
    ON_CALL(MockMove, GetDamageCategory).WillByDefault(Return(EMoveDamageCategory::Physical));
    ON_CALL(MockMove, GetBasePower).WillByDefault(Return(65));
    ON_CALL(MockMove, GetType).WillByDefault(Return(TEXT("ICE")));
    FMoveData MoveData;
    ON_CALL(MockMove, GetMoveData).WillByDefault(ReturnRef(MoveData));

    auto Move = NewObject<UCriticalTestMove>();
    Move->Initialize(Battle, BaseMove);
    auto [Damage, Effeciveness, CriticalHit] =
            IBattleMove::Execute_CalculateDamage(Move, User, Target, 1);
    UE_CHECK_EQUAL(EDamageEffectiveness::SuperEffective, Effeciveness);
    UE_CHECK_TRUE(CriticalHit);
    
    return true;
}