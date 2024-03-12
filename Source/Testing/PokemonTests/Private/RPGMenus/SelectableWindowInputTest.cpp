// "Unreal Pokémon" created by Retro & Chill.


#include "RPGMenus/SelectableWindowInputTest.h"

#include "IAutomationDriverModule.h"
#include "CommonDefines.h"
#include "IAutomationDriver.h"
#include "IDriverSequence.h"
#include "LocateBy.h"
#include "Utilities/AutomationDriverHandler.h"
#include "Utilities/InputUtilities.h"
#include "Windows/SelectableWidget.h"


// Sets default values
ASelectableWindowInputTest::ASelectableWindowInputTest() {
	OnTestStart.AddDynamic(this, &ASelectableWindowInputTest::RunTest);
}

void ASelectableWindowInputTest::RunTest() {
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
	NewWidget->OnConfirm.AddDynamic(this, &ASelectableWindowInputTest::OnConfirm);
	UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
	TEST_ASSERT(AssertFalse(NewIndex.IsSet(), TEXT("Confirm should not be called!")))
	
	NewWidget->SetActive(true);
	UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
	TEST_ASSERT(AssertTrue(NewIndex.IsSet(), TEXT("Confirm should be called!")))
	TEST_ASSERT(AssertEqual_Int(5, NewIndex.GetValue(), TEXT("Index should be 5!")));

	NewWidget->OnCancel.AddDynamic(this, &ASelectableWindowInputTest::OnCancel);
	UInputUtilities::SimulateKeyPress(NewWidget, CancelKey);
	TEST_ASSERT(AssertTrue(bCanceled, TEXT("Cancel should be called!")))

	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}

void ASelectableWindowInputTest::OnConfirm(int32 Index) {
	NewIndex.Emplace(Index);
}

void ASelectableWindowInputTest::OnCancel() {
	bCanceled = true;
}

