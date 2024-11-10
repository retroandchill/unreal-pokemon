
#include "Asserts.h"
#include "Components/Common/MoveInfoWindow.h"
#include "Components/DisplayText.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MoveInfoWindowTest, "Unit Tests.Windows.MoveInfoWindowTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MoveInfoWindowTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UMoveInfoWindow>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UMoveInfoWindow> MoveInfo(CreateWidget<UMoveInfoWindow>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(MoveInfo.Get());
    MoveInfo->AddToViewport();

    FIND_CHILD_WIDGET(MoveInfo.Get(), UDisplayText, PowerText);
    UE_ASSERT_NOT_NULL(PowerText);
    FIND_CHILD_WIDGET(MoveInfo.Get(), UDisplayText, AccuracyText);
    UE_ASSERT_NOT_NULL(AccuracyText);
    FIND_CHILD_WIDGET(MoveInfo.Get(), UDisplayText, DescriptionText);
    UE_ASSERT_NOT_NULL(DescriptionText);

    auto Move1 = NewObject<UDefaultMove>(World.Get())->Initialize({.Move = TEXT("AURASPHERE")});
    MoveInfo->SetMove(Move1);
    UE_ASSERT_EQUAL(TEXT("80"), PowerText->GetText().ToString());
    UE_ASSERT_EQUAL(TEXT("---"), AccuracyText->GetText().ToString());
    UE_ASSERT_EQUAL(
        TEXT("The user looses a blast of aura power from deep within its body. This move is certain to hit."),
        DescriptionText->GetText().ToString());

    auto Move2 = NewObject<UDefaultMove>(World.Get())->Initialize({.Move = TEXT("PSYWAVE")});
    MoveInfo->SetMove(Move2);
    UE_ASSERT_EQUAL(TEXT("???"), PowerText->GetText().ToString());
    UE_ASSERT_EQUAL(TEXT("100%"), AccuracyText->GetText().ToString());
    UE_ASSERT_EQUAL(TEXT("The target is attacked with an odd psychic wave. The attack varies in intensity."),
                    DescriptionText->GetText().ToString());

    auto Move3 = NewObject<UDefaultMove>(World.Get())->Initialize({.Move = TEXT("GROWL")});
    MoveInfo->SetMove(Move3);
    UE_ASSERT_EQUAL(TEXT("---"), PowerText->GetText().ToString());
    UE_ASSERT_EQUAL(TEXT("100%"), AccuracyText->GetText().ToString());
    UE_ASSERT_EQUAL(
        TEXT("The user growls in an endearing way, making the foe less wary. The foe's Attack stat is lowered."),
        DescriptionText->GetText().ToString());

    return true;
}