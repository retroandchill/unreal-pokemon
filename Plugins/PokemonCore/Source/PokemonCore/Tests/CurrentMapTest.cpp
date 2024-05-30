#if WITH_TESTS && HAS_AUTOMATION_HELPERS
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
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto &PokemonSubsystem = UPokemonSubsystem::GetInstance(World);
    PokemonSubsystem.SetCurrentLocation(FText::FromStringView(TEXT("Test Map")));
    FPokemonDTO Blank;
    auto BlankBlock = NewObject<UDefaultObtainedBlock>()->Initialize(Blank);
    ASSERT_TRUE(BlankBlock->GetObtainText().IsSet());
    CHECK_EQUAL(TEXT("Test Map"), BlankBlock->GetObtainText()->ToString());

    return true;
}
#endif