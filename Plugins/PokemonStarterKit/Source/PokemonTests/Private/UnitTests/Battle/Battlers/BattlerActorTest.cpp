#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include <thread>

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BattlerActorTest_Stats, "Unit Tests.Battle.Battlers.BattlerActorTest.Stats",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BattlerActorTest_Stats::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
    ON_CALL(MockSide, ShowBackSprites).WillByDefault(Return(true));

    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"), .Level = 50});
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->Initialize(Side, Pokemon);

    UE_CHECK_TRUE(Battler->GetOwningSide() == Side);
    UE_CHECK_EQUAL(Pokemon->GetNickname().ToString(), Battler->GetNickname().ToString());
    UE_CHECK_EQUAL(Pokemon->GetGender(), Battler->GetGender());
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetLevel(), Battler->GetPokemonLevel());
    UE_CHECK_EQUAL(Pokemon->GetCurrentHP(), Battler->GetHP());
    UE_CHECK_EQUAL(Pokemon->GetMaxHP(), Battler->GetMaxHP());
    UE_CHECK_EQUAL(1.f, Battler->GetHPPercent());
    UE_CHECK_FALSE(Battler->IsFainted());
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("ATTACK"))->GetStatValue(), Battler->GetAttack().StatValue);
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("DEFENSE"))->GetStatValue(), Battler->GetDefense().StatValue);
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_ATTACK"))->GetStatValue(), Battler->GetSpecialAttack().StatValue);
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPECIAL_DEFENSE"))->GetStatValue(),
                Battler->GetSpecialDefense().StatValue);
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetStat(TEXT("SPEED"))->GetStatValue(), Battler->GetSpeed().StatValue);
    UE_CHECK_EQUAL(Pokemon->GetStatBlock()->GetExpPercent(), Battler->GetExpPercent());

    auto &PokemonTypes = Pokemon->GetTypes();
    auto BattlerTypes = Battler->GetTypes();
    UE_ASSERT_EQUAL(PokemonTypes.Num(), BattlerTypes.Num());
    for (int32 i = 0; i < BattlerTypes.Num(); i++) {
        UE_CHECK_EQUAL(PokemonTypes[i], BattlerTypes[i]);
    }

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAiBattlerController, "Unit Tests.Battle.Battlers.TestAiBattlerController",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAiBattlerController::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
    ON_CALL(MockSide, ShowBackSprites).WillByDefault(Return(false));

    FString SelectedAction;
    ON_CALL(MockBattle, QueueAction).WillByDefault([&SelectedAction](const TUniquePtr<IBattleAction> &Action) {
        SelectedAction = Action->GetActionMessage().ToString();
    });

    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{.Species = TEXT("MIMIKYU"),
                    .Level = 50,
                    .Moves = {TEXT("SHADOWSNEAK"), TEXT("PLAYROUGH"), TEXT("SWORDSDANCE"), TEXT("SHADOWCLAW")}});
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->Initialize(Side, Pokemon);

    Battler->SelectActions();
    auto BusyWait = AsyncThread([&SelectedAction] {
        while (SelectedAction.IsEmpty()) {
            std::this_thread::yield();
        }
    });

    UE_ASSERT_TRUE(BusyWait.WaitFor(FTimespan::FromMinutes(2)));
    TArray<FString> PossibleMessages = {
        TEXT("Mimikyu used Shadow Sneak!"),
        TEXT("Mimikyu used Play Rough!"),
        TEXT("Mimikyu used Swords Dance!"),
        TEXT("Mimikyu used Shadow Claw!"),
    };
    UE_CHECK_TRUE(PossibleMessages.Contains(SelectedAction));

    return true;
}