#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SetHoldItemTest, "Unit Tests.Core.Pokemon.SetHoldItemTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SetHoldItemTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    UPokemonSubsystem::GetInstance(World.Get()).StartNewGame();
    auto Player = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    auto Pokemon = NewObject<UGamePokemon>(World.Get());
    Pokemon->Initialize({.Species = TEXT("PIKACHU"), .Level = 20}, Player);

    CHECK_NULL(Pokemon->GetHoldItem());
    Pokemon->SetHoldItem(TEXT("REPEL"));
    ASSERT_NOT_NULL(Pokemon->GetHoldItem());
    CHECK_EQUAL(TEXT("REPEL"), Pokemon->GetHoldItem()->ID.ToString());
    Pokemon->RemoveHoldItem();
    CHECK_NULL(Pokemon->GetHoldItem());

    return true;
}