#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Utilities/StatusEffectUtilities.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatusEffects, "Unit Tests.Pokemon.TestStatusEffects",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatusEffects::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    UPokemonSubsystem::GetInstance(World.Get()).StartNewGame();
    auto Player = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    auto Pokemon = NewObject<UGamePokemon>(World.Get());
    Pokemon->Initialize({.Species = TEXT("PIKACHU"), .Level = 20}, Player);

    FStatus Status;
    UE_CHECK_EQUAL(EStatusEffectStatus::NoStatusEffect, UStatusEffectUtilities::GetStatusEffect(Pokemon, Status));

    Pokemon->SetStatusEffect(FStatusHandle("BURN"));
    UE_CHECK_EQUAL(EStatusEffectStatus::HasStatusEffect, UStatusEffectUtilities::GetStatusEffect(Pokemon, Status));
    UE_CHECK_EQUAL(TEXT("BURN"), Status.ID.ToString());

    Pokemon->SetStatusEffect(FStatusHandle("SLEEP"));
    UE_CHECK_EQUAL(EStatusEffectStatus::HasStatusEffect, UStatusEffectUtilities::GetStatusEffect(Pokemon, Status));
    UE_CHECK_EQUAL(TEXT("BURN"), Status.ID.ToString());

    Pokemon->SetStatusEffect(FStatusHandle("SLEEP"), true);
    UE_CHECK_EQUAL(EStatusEffectStatus::HasStatusEffect, UStatusEffectUtilities::GetStatusEffect(Pokemon, Status));
    UE_CHECK_EQUAL(TEXT("SLEEP"), Status.ID.ToString());

    Pokemon->RemoveStatusEffect();
    UE_CHECK_EQUAL(EStatusEffectStatus::NoStatusEffect, UStatusEffectUtilities::GetStatusEffect(Pokemon, Status));

    return true;
}