// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonUI/MenuActionTest.h"

#include "RPGPlayerController.h"
#include "Actions/MenuAction.h"

#define TEST_ASSERT(Expression) if (!(Expression)) { FinishTest(EFunctionalTestResult::Failed, TEXT("Test failed!")); return; }

AMenuActionTest::AMenuActionTest() {
	OnTestStart.AddDynamic(this, &AMenuActionTest::RunTest);
}

void AMenuActionTest::RunTest() {
	TEST_ASSERT(AssertIsValid(MenuAction, TEXT("Screen class should be valid!")))

	auto PlayerController = Cast<ARPGPlayerController>(GetWorld()->GetFirstPlayerController());
	TEST_ASSERT(AssertIsValid(PlayerController, TEXT("The player should be is valid and of class RPG Player Controller!")))
	MenuAction->PerformAction(PlayerController->GetPawn());

	auto TopOfStack = PlayerController->GetTopOfStack();
	TEST_ASSERT(AssertIsValid(TopOfStack, TEXT("There should be a valid screen on the stack!")))

	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}
