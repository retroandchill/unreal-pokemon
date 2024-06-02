
#include "Asserts.h"
#include "Blueprint/WidgetTree.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Primatives/DisplayText.h"
#include "Screens/TrainerCardScreen.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TrainerCardTest, "Unit Tests.UI.TrainerCardTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TrainerCardTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UTrainerCardScreen>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UTrainerCardScreen> Screen(CreateWidget<UTrainerCardScreen>(World.Get(), WidgetClass));
    ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();

    auto TrainerNameText = Screen->WidgetTree->FindWidget<UDisplayText>(TEXT("TrainerNameText"));
    ASSERT_NOT_NULL(TrainerNameText);
    CHECK_EQUAL(Trainer->GetTrainerName().ToString(), TrainerNameText->GetText().ToString());

    auto IdText = Screen->WidgetTree->FindWidget<UDisplayText>(TEXT("IdText"));
    ASSERT_NOT_NULL(IdText);
    CHECK_EQUAL(Trainer->GetIdNumber(), FCString::Atoi(*IdText->GetText().ToString()));

    return true;
}