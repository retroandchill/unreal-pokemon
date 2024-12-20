﻿#include "Asserts.h"
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

    UE_CHECK_FALSE(Pokemon->GetHoldItem().IsSet());
    Pokemon->SetHoldItem(TEXT("REPEL"));
    UE_CHECK_TRUE(Pokemon->GetHoldItem().IsSet());
    UE_CHECK_EQUAL(TEXT("REPEL"), Pokemon->GetHoldItem()->ID.ToString());
    Pokemon->RemoveHoldItem();
    UE_CHECK_FALSE(Pokemon->GetHoldItem().IsSet());

    return true;
}