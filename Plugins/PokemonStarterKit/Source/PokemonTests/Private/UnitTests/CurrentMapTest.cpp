﻿
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CurrentMapTest, "Unit Tests.Core.CurrentMapTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CurrentMapTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto &PokemonSubsystem = UPokemonSubsystem::GetInstance(World.Get());
    PokemonSubsystem.SetCurrentLocation(FText::FromStringView(TEXT("Test Map")));
    FPokemonDTO Blank;
    auto BlankBlock = NewObject<UDefaultObtainedBlock>(World.Get())->Initialize(Blank);
    UE_ASSERT_TRUE(BlankBlock->GetObtainText().IsSet());
    UE_CHECK_EQUAL(TEXT("Test Map"), BlankBlock->GetObtainText()->ToString());

    return true;
}