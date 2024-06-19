
#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultObtainedBlockTest,
                                 "Unit Tests.Pokemon.TrainerMemo.DefaultObtainedBlockTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultObtainedBlockTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    FPokemonDTO Blank;
    auto BlankBlock = NewObject<UDefaultObtainedBlock>(World.Get())->Initialize(Blank);
    UE_CHECK_EQUAL(5, BlankBlock->GetLevelMet());
    UE_CHECK_EQUAL(EObtainMethod::Default, BlankBlock->GetObtainMethod());
    UE_ASSERT_TRUE(BlankBlock->GetObtainText().IsSet());
    UE_CHECK_EQUAL(TEXT(""), BlankBlock->GetObtainText().GetValue().ToString());

    FPokemonDTO DTO = {.Level = 10,
                       .ObtainMethod = EObtainMethod::FatefulEncounter,
                       .MetLocation = FText::FromStringView(TEXT("TestLocation"))};
    auto ObtainedBlock = NewObject<UDefaultObtainedBlock>(World.Get())->Initialize(DTO);
    UE_CHECK_EQUAL(10, ObtainedBlock->GetLevelMet());
    UE_CHECK_EQUAL(EObtainMethod::FatefulEncounter, ObtainedBlock->GetObtainMethod());
    UE_ASSERT_TRUE(ObtainedBlock->GetObtainText().IsSet());
    UE_CHECK_EQUAL(TEXT("TestLocation"), ObtainedBlock->GetObtainText().GetValue().ToString());
    UE_CHECK_NOT_NULL(ObtainedBlock->GetTimeReceived());
    UE_CHECK_FALSE(ObtainedBlock->GetHatchedMap().IsSet());
    UE_CHECK_NULL(ObtainedBlock->GetTimeHatched());

    return true;
}