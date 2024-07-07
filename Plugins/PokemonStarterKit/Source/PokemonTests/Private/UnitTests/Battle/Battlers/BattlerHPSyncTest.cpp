#include "Asserts.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BattlerHPSyncTest, "Unit Tests.Battle.Battlers.BattlerHPSyncTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BattlerHPSyncTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("MIMIKYU"),
            .Level = 100,
            .IVs = {
                {"HP", 31},
                {"ATTACK", 31},
                {"DEFENSE", 31},
                {"SPECIAL_ATTACK", 31},
                {"SPECIAL_DEFENSE", 31},
                {"SPEED", 31}
            },
            .EVs = {
                {"HP", 4},
                {"ATTACK", 252},
                {"SPEED", 252}
            },
            .Nature = FName("ADAMANT")
        });

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
    
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->Initialize(Side, Pokemon);

    auto AbilityComponent = Battler->GetAbilityComponent();
    AbilityComponent->SetNumericAttributeBase(UPokemonCoreAttributeSet::GetHPAttribute(), 200.f);
    UE_CHECK_EQUAL(200, Pokemon->GetCurrentHP());
    
    AbilityComponent->SetNumericAttributeBase(UPokemonCoreAttributeSet::GetHPAttribute(), 300.f);
    UE_CHECK_EQUAL(252.f, AbilityComponent->GetCoreAttributes()->GetHP());
    UE_CHECK_EQUAL(252, Pokemon->GetCurrentHP());

    
    AbilityComponent->SetNumericAttributeBase(UPokemonCoreAttributeSet::GetHPAttribute(), -1.f);
    UE_CHECK_EQUAL(0.f, AbilityComponent->GetCoreAttributes()->GetHP());
    UE_CHECK_EQUAL(0, Pokemon->GetCurrentHP());
    
    return true;
}