#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
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
#include "TestAdapter.h"
#include <thread>

using namespace testing;

BEGIN_DEFINE_SPEC(FBattlerActorTest, "Unit Tests.Battle.Battlers.BattlerActorTest",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
    CORO_FUNCTIONS()

END_DEFINE_SPEC(FBattlerActorTest);

void FBattlerActorTest::Define() {
    CoroIt("Stats", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

        CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
        CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
        ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
        ON_CALL(MockSide, ShowBackSprites).WillByDefault(Return(true));

        auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"), .Level = 50});
        auto Battler = World->SpawnActor<ATestBattlerActor>();
        Battler->DispatchBeginPlay(false);
        co_await Battler->Initialize(Side, Pokemon);

        CHECK(Battler->GetOwningSide() == Side);
        CHECK(Pokemon->GetNickname().ToString() == Battler->GetNickname().ToString());
        CHECK(Pokemon->GetGender() == Battler->GetGender());
        CHECK(Pokemon->GetStatBlock()->GetLevel() == Battler->GetPokemonLevel());

        auto CoreAttributes = Battler->GetAbilityComponent()->GetCoreAttributes();
        CHECK(Pokemon->GetCurrentHP() == static_cast<int32>(CoreAttributes->GetHP()));
        CHECK(Pokemon->GetMaxHP() == static_cast<int32>(CoreAttributes->GetMaxHP()));
        CHECK_EQUALS("Battler HP", Battler->GetHPPercent(), 1.f);
        CHECK(Pokemon->GetStatBlock()->GetStat("ATTACK")->GetStatValue() == static_cast<int32>(CoreAttributes->GetAttack()));
        CHECK(Pokemon->GetStatBlock()->GetStat("DEFENSE")->GetStatValue() == static_cast<int32>(CoreAttributes->GetDefense()));
        CHECK(Pokemon->GetStatBlock()->GetStat("SPECIAL_ATTACK")->GetStatValue() == static_cast<int32>(CoreAttributes->GetSpecialAttack()));
        CHECK(Pokemon->GetStatBlock()->GetStat("SPECIAL_DEFENSE")->GetStatValue() == static_cast<int32>(CoreAttributes->GetSpecialDefense()));
        CHECK(Pokemon->GetStatBlock()->GetStat("SPEED")->GetStatValue() == static_cast<int32>(CoreAttributes->GetSpeed()));
        CHECK_FALSE(Battler->IsFainted());
        CHECK("Exp Percent", Battler->GetExpPercent(), Pokemon->GetStatBlock()->GetExpPercent());

        auto &PokemonTypes = Pokemon->GetTypes();
        auto BattlerTypes = Battler->GetTypes();
        CO_REQUIRE(PokemonTypes.Num() == BattlerTypes.Num());
        for (int32 i = 0; i < BattlerTypes.Num(); i++) {
            CHECK(PokemonTypes[i] == BattlerTypes[i]);
        }
    });
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
        World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"),
                                 .Level = 50,
                                 .Moves = {{.Move = TEXT("SHADOWSNEAK")},
                                           {.Move = TEXT("PLAYROUGH")},
                                           {.Move = TEXT("SWORDSDANCE")},
                                           {.Move = TEXT("SHADOWCLAW")}}});
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->DispatchBeginPlay(false);
    Battler->Initialize(Side, Pokemon).Wait();

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