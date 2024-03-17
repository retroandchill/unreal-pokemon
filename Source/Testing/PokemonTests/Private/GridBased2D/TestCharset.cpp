// "Unreal Pokémon" created by Retro & Chill.


#include "GridBased2D/TestCharset.h"

#include "CommonDefines.h"
#include "PaperFlipbook.h"
#include "Characters/FacingDirection.h"
#include "Characters/Charset.h"


// Sets default values
ATestCharset::ATestCharset() {
	OnTestStart.AddDynamic(this, &ATestCharset::RunTest);
}

void ATestCharset::RunTest() {
	using enum EFacingDirection;
	TEST_ASSERT(AssertIsValid(Charset, TEXT("Charset asset must be valid")))

	TEST_ASSERT(AssertEqual_Object(Charset->GetSprite(Down), Charset->GetDownSprite(), TEXT("Down Sprite")))
	TEST_ASSERT(AssertEqual_Object(Charset->GetSprite(Up), Charset->GetUpSprite(), TEXT("Up Sprite")))
	TEST_ASSERT(AssertEqual_Object(Charset->GetSprite(Left), Charset->GetLeftSprite(), TEXT("Left Sprite")))
	TEST_ASSERT(AssertEqual_Object(Charset->GetSprite(Right), Charset->GetRightSprite(), TEXT("Right Sprite")))

	auto InvalidDirection = static_cast<EFacingDirection>(static_cast<uint8>(Up) + static_cast<uint8>(10));
	TEST_ASSERT(AssertTrue(Charset->GetSprite(InvalidDirection) == nullptr, TEXT("Invalid Direction")))
	TEST_ASSERT(AssertFalse(Charset->CanStopOnFrame(InvalidDirection, 0), TEXT("Can't stop on invalid")))

	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}


