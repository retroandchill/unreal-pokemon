#include "AbilitySystemBlueprintLibrary.h"
#include "Asserts.h"
#include "TestAdapter.h"
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

BEGIN_DEFINE_SPEC(FTestAddingAndRemovingStatusEffects, "Unit Tests.Battle.StatusEffects.TestAddingAndRemovingStatusEffects",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
CORO_FUNCTIONS()

END_DEFINE_SPEC(FTestAddingAndRemovingStatusEffects);

void FTestAddingAndRemovingStatusEffects::Define() {
    CoroIt("AddAndRemoveStatusEffects", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GLACEON"),
                                                                                     .Level = 75,
                                                                                     .IVs = {{"ATTACK", 31}},
                                                                                     .EVs = {{"ATTACK", 104}},
                                                                                     .Nature = FName("TIMID"),
                                                                                     .Moves = {{.Move = TEXT("ICEFANG")}}});

        CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
        CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
        ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));

        auto Battler = World->SpawnActor<ATestBattlerActor>();
        co_await Battler->Initialize(Side, Pokemon1);

        auto AbilityComponent = Battler->GetAbilityComponent();
        auto StatusEffect = LoadClass<UGameplayEffect>(
            nullptr, TEXT("/Game/Blueprints/Battle/StatusEffects/StatusEffect_BURN.StatusEffect_BURN_C"));
        CO_REQUIRE(StatusEffect != nullptr);
        auto Spec = AbilityComponent->MakeOutgoingSpec(StatusEffect, 1, AbilityComponent->MakeEffectContext());
        auto ActiveEffect = AbilityComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);

        CO_REQUIRE(Battler->GetStatusEffect().IsSet());
        UE_CHECK_EQUAL(TEXT("BURN"), Battler->GetStatusEffect()->StatusEffectID.ToString());

        AbilityComponent->RemoveActiveGameplayEffect(ActiveEffect);
        UE_CHECK_FALSE(Battler->GetStatusEffect().IsSet());
    });
}