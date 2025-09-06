#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Components/PokemonActionOptions.h"
#include "Components/RPGButtonBase.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattler.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonActionOptions, "Unit Tests.Battle.UI.TestPokemonActionOptions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonActionOptions::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonActionOptions>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    CREATE_MOCK(IBattler, Battler, FMockBattler, MockBattler);

    TWidgetPtr<UPokemonActionOptions> Window(CreateWidget<UPokemonActionOptions>(World.Get(), WidgetClass));
    Window->AddToViewport();
    Window->SetBattler(Battler);

    auto Children = UWidgetTestUtilities::FindAllChildWidgetsOfType<URPGButtonBase>(Window.Get());
    UE_CHECK_EQUAL(Window->GetItemCount(), Children.Num());

    return true;
}