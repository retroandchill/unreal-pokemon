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

#include "Utilities/fakeit.hpp"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySwitchHandlerTest, "Project.UI.PartySwitchHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartySwitchHandlerTest::RunTest(const FString& Parameters) {
	UGameInstance* GameInstance = nullptr;
	if (!UPokemonSubsystem::Exists()) {
		GameInstance = NewObject<UGameInstance>();
		GameInstance->Init();
	}
	
	Mock<IPartyScreen> Screen;
	Fake(Method(Screen, BeginSwitch));
	
	auto Pokemon1 = UGamePokemon::Create({.Species = TEXT("RIOLU"), .Level = 5 });
	auto Pokemon2 = UGamePokemon::Create({.Species = TEXT("OSHAWOTT"), .Level = 5 });
	//Fake(Dtor(Pokemon2));
	
	TArray<TScriptInterface<IPokemon>> Party;
	Party.Emplace(Pokemon1);
	
	TGCPointer<UPartyMenuHandler> Handler(NewObject<UPartySwitchHandler>());
	bool Passed = TestFalse(TEXT("Command should not be visible!"), Handler->ShouldShow(Screen.get(), Party, 0));
	
	Party.Emplace(Pokemon2);
	Passed &= TestTrue(TEXT("Command should be visible!"), Handler->ShouldShow(Screen.get(), Party, 0));

	Handler->Handle(Screen.get(), Party, 0);
	Passed &= Verify(Method(Screen, BeginSwitch).Using(0));

	if (GameInstance != nullptr) {
		GameInstance->Shutdown();
	}
	return Passed;
}
