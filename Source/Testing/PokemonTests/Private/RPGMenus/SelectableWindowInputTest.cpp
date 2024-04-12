// "Unreal Pokémon" created by Retro & Chill.

#include "RPGMenus/SelectableWindowInputTest.h"

#include "CommonDefines.h"
#include "IAutomationDriver.h"
#include "IAutomationDriverModule.h"
#include "IDriverSequence.h"
#include "LocateBy.h"
#include "Utilities/AutomationDriverHandler.h"
#include "Utilities/InputUtilities.h"
#include "Windows/SelectableWidget.h"

void ASelectableWindowInputTest::BeginPlay() {
    Super::BeginPlay();
    OnTestStart.AddDynamic(this, &ASelectableWindowInputTest::TestRun);
}

void ASelectableWindowInputTest::TestRun() {
    NewIndex.Reset();
    bCanceled = false;

    TEST_ASSERT(AssertIsValid(WidgetClass, TEXT("Widget class should be valid!")))

    auto NewWidget = CreateWidget<USelectableWidget>(GetGameInstance(), WidgetClass);
    TEST_ASSERT(AssertIsValid(NewWidget, TEXT("Widget should get created should be valid!")))
    NewWidget->AddToViewport();

    NewWidget->SetActive(true);
    NewWidget->SetIndex(0);

    TEST_ASSERT(AssertIsValid(NewWidget, TEXT("Widget should get created should be valid!")))
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetItemCount(), 6, TEXT("Widget should have 6 items!")));
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetRowCount(), 3, TEXT("Widget should have 3 rows!")));
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetColumnCount(), 2, TEXT("Widget should have 2 columns!")));
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetColumn(4), 1, TEXT("Column for Index 4")))
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetRow(4), 1, TEXT("Row for Index 4")))
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetColumn(5), 2, TEXT("Column for Index 5")))
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetRow(5), 1, TEXT("Row for Index 5")))
    
    UInputUtilities::SimulateKeyPress(NewWidget, DownKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 2, TEXT("Index should be 2!")));

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 0, TEXT("Index should be 0!")));

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 4, TEXT("Index should be 5!")));

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 3, TEXT("Index should be 3!")));

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 4, TEXT("Index should be 4!")));

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 5, TEXT("Index should be 5!")));

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 4, TEXT("Index should be 4!")));

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 0, TEXT("Index should be 0!")));

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), 5, TEXT("Index should be 3!")));

    NewWidget->SetActive(false);
    NewWidget->GetOnConfirm().AddDynamic(this, &ASelectableWindowInputTest::OnConfirm);
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    TEST_ASSERT(AssertFalse(NewIndex.IsSet(), TEXT("Confirm should not be called!")))

    NewWidget->SetActive(true);
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    TEST_ASSERT(AssertTrue(NewIndex.IsSet(), TEXT("Confirm should be called!")))
    TEST_ASSERT(AssertEqual_Int(5, NewIndex.GetValue(), TEXT("Index should be 5!")));

    NewWidget->SetActive(true);
    TEST_ASSERT(AssertTrue(NewWidget->IsActive(), TEXT("If the widget is active we shouldn't change states.")))
    
    NewWidget->GetOnCancel().AddDynamic(this, &ASelectableWindowInputTest::OnCancel);
    UInputUtilities::SimulateKeyPress(NewWidget, CancelKey);
    TEST_ASSERT(AssertTrue(bCanceled, TEXT("Cancel should be called!")))

    NewWidget->Deselect();
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), -1, TEXT("Deselection should be index -1")))
    
    NewWidget->Deselect();
    TEST_ASSERT(AssertEqual_Int(NewWidget->GetIndex(), -1, TEXT("Deselection should be index -1")))
    
    FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}

void ASelectableWindowInputTest::OnConfirm(int32 Index) { NewIndex.Emplace(Index); }

void ASelectableWindowInputTest::OnCancel() { bCanceled = true; }
