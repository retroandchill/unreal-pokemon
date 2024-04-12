// "Unreal Pokémon" created by Retro & Chill.

#include "RPGMenus/SelectableWidgetMouseTest.h"

#include "CommonDefines.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Primatives/SelectableOption.h"
#include "Utilities/SelectableOptionTestWidget.h"
#include "Utilities/WidgetChildUtilities.h"


void ASelectableWidgetMouseTest::BeginPlay() {
    Super::BeginPlay();
    OnTestStart.AddDynamic(this, &ASelectableWidgetMouseTest::TestRun);
}

void ASelectableWidgetMouseTest::TestRun() {
    SelectedIndex.Reset();
    auto NewWidget = CreateWidget<USelectableOptionTestWidget>(GetGameInstance(), WidgetClass);
    TEST_ASSERT(AssertIsValid(NewWidget, TEXT("Widget should get created should be valid!")))
    NewWidget->CreateChildWidgets();
    NewWidget->AddToViewport();
    NewWidget->OnConfirm.AddDynamic(this, &ASelectableWidgetMouseTest::HandledConfirm);

    auto &Options = NewWidget->GetOptions();

    auto Button1 = Cast<UButton>(UWidgetChildUtilities::FindChildWidget(Options[1], TEXT("PrimaryButton")));
    TEST_ASSERT(AssertIsValid(Button1, TEXT("Button should be locatable")))
    Button1->OnHovered.Broadcast();
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), -1, TEXT("")))

    NewWidget->SetActive(true);
    Button1->OnHovered.Broadcast();
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 1, TEXT("")))

    auto Button2 = Cast<UButton>(UWidgetChildUtilities::FindChildWidget(Options[2], TEXT("PrimaryButton")));
    TEST_ASSERT(AssertIsValid(Button2, TEXT("Button should be locatable")))

    
    NewWidget->SetActive(false);
    TEST_ASSERT(AssertFalse(SelectedIndex.IsSet(), TEXT("")))

    NewWidget->SetActive(true);
    Button2->OnClicked.Broadcast();
    TEST_ASSERT(AssertTrue(SelectedIndex.IsSet(), TEXT("")))
    TEST_ASSERT(AssertEqual_Int(2, SelectedIndex.GetValue(), TEXT("")))

    FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}
void ASelectableWidgetMouseTest::HandledConfirm(int32 Index) {
    SelectedIndex.Emplace(Index);
}

