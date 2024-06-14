#include "Asserts.h"
#include "TestBattlerActor.h"
#include "TestSpriteActor.h"
#include "Battle/Battle.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Battle/BattleSide.h"
#include "External/accessor.hpp"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"

using namespace accessor;
using namespace fakeit;

MEMBER_ACCESSOR(AccessBattleSpriteActor, ABattlerActor, BattlerSpriteClass, TSoftClassPtr<AActor>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BattlerActorTest_Stats, "Unit Tests.Battle.Battlers.BattlerActorTest.Stats",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BattlerActorTest_Stats::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [Side, MockSide] = UnrealMock::CreateMock<IBattleSide>();
    When(Method(MockSide, GetOwningBattle)).AlwaysReturn(Battle);
    When(Method(MockSide, ShowBackSprites)).AlwaysReturn(true);
    
    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"), .Level = 50});
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    accessMember<AccessBattleSpriteActor>(*Battler).get() = ATestSpriteActor::StaticClass();
    Battler->Initialize(Side, Pokemon);

    CHECK_TRUE(Battler->GetOwningSide() == Side);
    CHECK_EQUAL(Pokemon->GetNickname().ToString(), Battler->GetNickname().ToString());
    CHECK_EQUAL(Pokemon->GetGender(), Battler->GetGender());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetLevel(), Battler->GetPokemonLevel());
    CHECK_EQUAL(Pokemon->GetCurrentHP(), Battler->GetHP());
    CHECK_EQUAL(Pokemon->GetMaxHP(), Battler->GetMaxHP());
    CHECK_EQUAL(1.f, Battler->GetHPPercent());
    CHECK_FALSE(Battler->IsFainted());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("ATTACK"))->GetStatValue(), Battler->GetAttack());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("DEFENSE"))->GetStatValue(), Battler->GetDefense());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_ATTACK"))->GetStatValue(), Battler->GetSpecialAttack());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_DEFENSE"))->GetStatValue(), Battler->GetSpecialDefense());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPEED"))->GetStatValue(), Battler->GetSpeed());
    CHECK_EQUAL(Pokemon->GetStatBlock()->GetExpPercent(), Battler->GetExpPercent());

    auto &PokemonTypes = Pokemon->GetTypes();
    auto BattlerTypes = Battler->GetTypes();
    ASSERT_EQUAL(PokemonTypes.Num(), BattlerTypes.Num());
    for (int32 i = 0; i < BattlerTypes.Num(); i++) {
        CHECK_EQUAL(PokemonTypes[i], BattlerTypes[i]);
    }
    
    return true;
}