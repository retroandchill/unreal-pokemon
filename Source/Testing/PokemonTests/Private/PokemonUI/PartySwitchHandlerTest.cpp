// "Unreal Pokémon" created by Retro & Chill.
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Managers/PokemonSubsystem.h"
#include "Memory/GCPointer.h"
#include "Memory/RootMemoryPointers.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Screens/PartyScreen.h"
#include "Pokemon/Pokemon.h"
#include "Trainers/BasicTrainer.h"

#include "Utilities/fakeit.hpp"
#include "Utilities/PokemonTestUtilities.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySwitchHandlerTest, "Project.UI.PartySwitchHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartySwitchHandlerTest::RunTest(const FString& Parameters) {
	FGameInstancePtr GameInstance;
	if (!UPokemonSubsystem::Exists()) {
		GameInstance.Reset(NewObject<UGameInstance>());
		GameInstance->Init();
	}
	
	Mock<IPartyScreen> Screen;
	Fake(Method(Screen, BeginSwitch));

	auto Trainer = NewObject<UBasicTrainer>()
		->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
	Trainer->AddPokemonToParty(UGamePokemon::Create({.Species = TEXT("RIOLU"), .Level = 5 }));
	
	TGCPointer<UPartyMenuHandler> Handler(NewObject<UPartySwitchHandler>());
	bool Passed = TestFalse(TEXT("Command should not be visible!"), Handler->ShouldShow(Screen.get(), *Trainer, 0));
	
	Trainer->AddPokemonToParty(UGamePokemon::Create({.Species = TEXT("OSHAWOTT"), .Level = 5 }));
	Passed &= TestTrue(TEXT("Command should be visible!"), Handler->ShouldShow(Screen.get(), *Trainer, 0));

	Handler->Handle(Screen.get(), *Trainer, 0);
	Passed &= Verify(Method(Screen, BeginSwitch).Using(0));
	
	return Passed;
}
