// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonUI/TrainerCardTest.h"
#include "Blueprint/WidgetTree.h"
#include "CommonDefines.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Primatives/DisplayText.h"
#include "Screens/TrainerCardScreen.h"
#include "Utilities/PokemonTestUtilities.h"
#include "Utilities/RAII.h"
#include "Windows/SelectableWidget.h"

void ATrainerCardTest::BeginPlay() {
    Super::BeginPlay();
    OnTestStart.AddDynamic(this, &ATrainerCardTest::TestRun);
}

void ATrainerCardTest::TestRun() {
    TEST_ASSERT(AssertIsValid(WidgetClass, TEXT("Widget class should be valid!")))

    TWidgetPtr<UTrainerCardScreen> Screen(CreateWidget<UTrainerCardScreen>(GetGameInstance(), WidgetClass));
    TEST_ASSERT(AssertIsValid(Screen.Get(), TEXT("Widget should get created should be valid!")))
    Screen->AddToViewport();

    auto Trainer = UPokemonSubsystem::GetInstance().GetPlayer();

    auto TrainerNameText = Screen->WidgetTree->FindWidget<UDisplayText>(TEXT("TrainerNameText"));
    TEST_ASSERT(AssertIsValid(TrainerNameText, TEXT("TrainerNameText should exist!")))
    TEST_ASSERT(
        AssertEqual_String(TrainerNameText->GetText().ToString(), Trainer->GetTrainerName().ToString(), TEXT("")))

    auto IdText = Screen->WidgetTree->FindWidget<UDisplayText>(TEXT("IdText"));
    TEST_ASSERT(AssertIsValid(IdText, TEXT("IdText should exist!")))
    TEST_ASSERT(AssertEqual_Int(FCString::Atoi(*IdText->GetText().ToString()), Trainer->GetIdNumber(), TEXT("")))

    FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}
