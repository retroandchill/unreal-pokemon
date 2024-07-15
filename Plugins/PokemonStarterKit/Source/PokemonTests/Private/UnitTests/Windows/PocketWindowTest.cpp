﻿#include "Windows/PocketWindow.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Components/DisplayText.h"
#include "Settings/BaseSettings.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PocketWindowTest, "Unit Tests.Windows.PocketWindowTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PocketWindowTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPocketWindow>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UPocketWindow> PocketWindow(CreateWidget<UPocketWindow>(World.Get(), WidgetClass));
    FIND_CHILD_WIDGET(PocketWindow.Get(), UDisplayText, PocketName);
    UE_ASSERT_NOT_NULL(PocketName);

    auto &Settings = Pokemon::FBaseSettings::Get();
    for (auto &PocketSettings = Settings.GetPocketInfo(); auto &[Key, Value] : PocketSettings) {
        PocketWindow->SetCurrentPocket(Key);
        UE_CHECK_EQUAL(Value.DisplayName.ToString(), PocketName->GetText().ToString());
    }

    return true;
}