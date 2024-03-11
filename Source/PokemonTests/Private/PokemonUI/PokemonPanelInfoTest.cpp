// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonUI/PokemonPanelInfoTest.h"
#include "CommonDefines.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"
#include "Utilities/PokemonTestUtilities.h"
#include "Windows/PokemonSelectionPane.h"

// Sets default values
APokemonPanelInfoTest::APokemonPanelInfoTest() {
	OnTestStart.AddDynamic(this, &APokemonPanelInfoTest::RunTest);
}

void APokemonPanelInfoTest::RunTest() {
	TEST_ASSERT(AssertIsValid(ScreenClass, TEXT("Screen class should be set!")))
	UPokemonTestUtilities::CreateMockParty(this);
	
	auto Screen = CreateWidget<UPokemonSelectionPane>(GetGameInstance(), ScreenClass);
	Screen->AddToViewport();

	auto ContentsArea = Screen->WidgetTree->FindWidget<UCanvasPanel>(TEXT("ContentsArea"));
	TEST_ASSERT(AssertIsValid(ContentsArea, TEXT("Contents Area should exist!")))

	TArray<UPokemonPanel*> Panels;
	for (int32 i = 0; i < 3; i++) {
		FName Name(FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}})));
		int32 OutIndex;
		auto Panel = Cast<UPokemonPanel>(Screen->WidgetTree->FindWidgetChild(ContentsArea, Name, OutIndex));
		TEST_ASSERT(AssertIsValid(ContentsArea, TEXT("Panel should exist!")))
		Panels.Add(Panel);
	}

	TEST_ASSERT(AssertEqual_Name(Panels[0]->GetPokemon()->GetSpecies().ID, TEXT("SAMUROTT"), TEXT("First Pokémon should be a Samurott!")))
	TEST_ASSERT(AssertEqual_Name(Panels[1]->GetPokemon()->GetSpecies().ID, TEXT("EMBOAR"), TEXT("Second Pokémon should be an Emboar!")))
	TEST_ASSERT(AssertEqual_Name(Panels[2]->GetPokemon()->GetSpecies().ID, TEXT("SERPERIOR"), TEXT("Thrid Pokémon should be a Serperior!")))

	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test passed!"));
}

