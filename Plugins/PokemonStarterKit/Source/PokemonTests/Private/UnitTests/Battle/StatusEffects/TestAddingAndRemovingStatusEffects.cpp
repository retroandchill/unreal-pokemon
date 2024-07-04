#include "AbilitySystemBlueprintLibrary.h"
#include "Asserts.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include "Pokemon/PokemonDTO.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestAddingAndRemovingStatusEffects, "Unit Tests.Battle.StatusEffects.TestAddingAndRemovingStatusEffects",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestAddingAndRemovingStatusEffects::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("GLACEON"),
            .Level = 75,
            .IVs = {{"ATTACK", 31}},
            .EVs = {{"ATTACK", 104}},
            .Nature = FName("TIMID"),
            .Moves = { TEXT("ICEFANG") }
        });

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
    
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->Initialize(Side, Pokemon1);

    auto AbilityComponent = Battler->GetAbilityComponent();
    auto StatusEffect = LoadClass<UGameplayEffect>(nullptr, TEXT("/Game/Blueprints/Battle/StatusEffects/StatusEffect_BURN.StatusEffect_BURN_C"));
    UE_ASSERT_NOT_NULL(StatusEffect);
    auto Spec = AbilityComponent->MakeOutgoingSpec(StatusEffect, 1, AbilityComponent->MakeEffectContext());
    auto ActiveEffect = AbilityComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);

    UE_ASSERT_TRUE(Battler->GetStatusEffect().IsSet());
    UE_CHECK_EQUAL(TEXT("BURN"), Battler->GetStatusEffect()->StatusEffectID.ToString());

    AbilityComponent->RemoveActiveGameplayEffect(ActiveEffect);
    UE_CHECK_FALSE(Battler->GetStatusEffect().IsSet());
    
    return true;
}