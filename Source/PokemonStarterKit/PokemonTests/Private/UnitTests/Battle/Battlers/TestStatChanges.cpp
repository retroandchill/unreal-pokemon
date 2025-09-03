#include "Asserts.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Stats/StatChangeHelpers.h"
#include "Battle/Stats/StatTags.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "TestAdapter.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"

using namespace testing;

BEGIN_DEFINE_SPEC(FTestStatChanges, "Unit Tests.Battle.Battlers.TestStatChanges",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
CORO_FUNCTIONS()
END_DEFINE_SPEC(FTestStatChanges)

void FTestStatChanges::Define() {
    Describe("StatChanges", [this] {
        CoroIt("Unaltered", [this]() -> UE5Coro::TCoroutine<> {
            auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
            auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
                World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"),
                                         .Level = 100,
                                         .IVs = {{"HP", 31},
                                                 {"ATTACK", 31},
                                                 {"DEFENSE", 31},
                                                 {"SPECIAL_ATTACK", 31},
                                                 {"SPECIAL_DEFENSE", 31},
                                                 {"SPEED", 31}},
                                         .EVs = {{"HP", 4}, {"ATTACK", 252}, {"SPEED", 252}},
                                         .Nature = FName("ADAMANT")});

            CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
            CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
            ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));

            auto Battler = World->SpawnActor<ATestBattlerActor>();
            co_await Battler->Initialize(Side, Pokemon1);

            auto AbilityComponent = Battler->GetAbilityComponent();
            auto CoreAttributes = AbilityComponent->GetCoreAttributes();
            UE_CHECK_EQUAL(306.f, CoreAttributes->GetAttack());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetAttackStagesAttribute(), 2);
            UE_CHECK_EQUAL(612.f, CoreAttributes->GetAttack());

            UE_CHECK_EQUAL(291.f, CoreAttributes->GetSpeed());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetSpeedStagesAttribute(), -1);
            UE_CHECK_EQUAL(194.f, CoreAttributes->GetSpeed());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetSpeedStagesAttribute(), 1);
            UE_CHECK_EQUAL(436.f, CoreAttributes->GetSpeed());
        });

        CoroIt("IgnoreTags", [this]() -> UE5Coro::TCoroutine<> {
            auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
            auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
                World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"),
                                         .Level = 100,
                                         .IVs = {{"HP", 31},
                                                 {"ATTACK", 31},
                                                 {"DEFENSE", 31},
                                                 {"SPECIAL_ATTACK", 31},
                                                 {"SPECIAL_DEFENSE", 31},
                                                 {"SPEED", 31}},
                                         .EVs = {{"HP", 4}, {"ATTACK", 252}, {"SPEED", 252}},
                                         .Nature = FName("ADAMANT")});

            CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
            CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
            ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));

            auto Battler = World->SpawnActor<ATestBattlerActor>();
            co_await Battler->Initialize(Side, Pokemon1);

            auto AbilityComponent = Battler->GetAbilityComponent();
            auto CoreAttributes = AbilityComponent->GetCoreAttributes();
            UE_CHECK_EQUAL(306.f, CoreAttributes->GetAttack());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetAttackStagesAttribute(), -2);
            UE_CHECK_EQUAL(153.f, CoreAttributes->GetAttack());

            auto &Lookup = Pokemon::Battle::Stats::FLookup::Get();
            AbilityComponent->AddLooseGameplayTag(Lookup.GetIgnoreNegativeTag("ATTACK"));
            UE_CHECK_EQUAL(306.f, CoreAttributes->GetAttack());

            UE_CHECK_EQUAL(196.f, CoreAttributes->GetDefense());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetDefenseStagesAttribute(), 2);
            UE_CHECK_EQUAL(392.f, CoreAttributes->GetDefense());
            AbilityComponent->AddLooseGameplayTag(Lookup.GetIgnorePositiveTag("DEFENSE"));
            UE_CHECK_EQUAL(196.f, CoreAttributes->GetDefense());
        });

        CoroIt("ApplyEffect", [this]() -> UE5Coro::TCoroutine<> {
            auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
            auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
                World.Get(), FPokemonDTO{.Species = TEXT("MIMIKYU"),
                                         .Level = 100,
                                         .IVs = {{"HP", 31},
                                                 {"ATTACK", 31},
                                                 {"DEFENSE", 31},
                                                 {"SPECIAL_ATTACK", 31},
                                                 {"SPECIAL_DEFENSE", 31},
                                                 {"SPEED", 31}},
                                         .EVs = {{"HP", 4}, {"ATTACK", 252}, {"SPEED", 252}},
                                         .Nature = FName("ADAMANT")});

            CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
            CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
            ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));

            auto Battler = World->SpawnActor<ATestBattlerActor>();
            co_await Battler->Initialize(Side, Pokemon1);

            auto AbilityComponent = Battler->GetAbilityComponent();
            auto StatStagesComponent = AbilityComponent->GetStatStages();
            UE_CHECK_EQUAL(2, co_await UStatChangeHelpers::ChangeBattlerStatStages(Battler, "ATTACK", 2));
            UE_CHECK_EQUAL(2.f, StatStagesComponent->GetAttackStages());

            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetAttackStagesAttribute(), 5.f);
            UE_CHECK_EQUAL(1, co_await UStatChangeHelpers::ChangeBattlerStatStages(Battler, "ATTACK", 2));
            UE_CHECK_EQUAL(6.f, StatStagesComponent->GetAttackStages());

            UE_CHECK_EQUAL(-7, co_await UStatChangeHelpers::ChangeBattlerStatStages(Battler, "ATTACK", -7));
            UE_CHECK_EQUAL(-1.f, StatStagesComponent->GetAttackStages());
            AbilityComponent->SetNumericAttributeBase(UStatStagesAttributeSet::GetAttackStagesAttribute(), -4.f);
            UE_CHECK_EQUAL(-2, co_await UStatChangeHelpers::ChangeBattlerStatStages(Battler, "ATTACK", -3));
            UE_CHECK_EQUAL(-6.f, StatStagesComponent->GetAttackStages());
        });
    });
}