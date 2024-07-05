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
#include "Utilities/WidgetTestUtilities.h"
#include "Pokemon/PokemonDTO.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChanges, "Unit Tests.Battle.Battlers.TestStatChanges.Unaltered",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChanges::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
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
    Battler->Initialize(Side, Pokemon1);

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
    
    return true;
}