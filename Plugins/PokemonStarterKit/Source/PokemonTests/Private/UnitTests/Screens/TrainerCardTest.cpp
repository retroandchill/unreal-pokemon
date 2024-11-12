
#include "Asserts.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DisplayText.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Screens/TrainerCardScreen.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TrainerCardTest, "Unit Tests.UI.TrainerCardTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TrainerCardTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UTrainerCardScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UTrainerCardScreen> Screen(CreateWidget<UTrainerCardScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    auto &Subsystem = UPokemonSubsystem::GetInstance(World.Get());
    UE_CHECK_TRUE(Screen->GetPlayerTrainer() == Subsystem.GetPlayer());
    UE_CHECK_TRUE(Screen->GetPlayerMetadata() == Subsystem.GetPlayerMetadata());

    return true;
}