#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Components/BattleMenuOption.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/PokemonActionOptions.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonActionOptions, "Unit Tests.Battle.UI.TestPokemonActionOptions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonActionOptions::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonActionOptions>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Battler, MockBattler] = UnrealMock::CreateMock<IBattler>();

    TWidgetPtr<UPokemonActionOptions> Window(CreateWidget<UPokemonActionOptions>(World.Get(), WidgetClass));
    Window->AddToViewport();
    Window->SetBattler(Battler);

    auto Children = UWidgetTestUtilities::FindAllChildWidgetsOfType<UBattleMenuOption>(Window.Get());
    CHECK_EQUAL(Window->GetItemCount(), Children.Num());

    return true;
}