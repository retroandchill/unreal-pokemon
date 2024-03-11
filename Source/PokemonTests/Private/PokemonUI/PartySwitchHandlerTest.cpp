// "Unreal Pokémon" created by Retro & Chill.
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Memory/RootMemoryPointers.h"
#include "Misc/AutomationTest.h"
#include "Screens/PartyScreen.h"
#include "Pokemon/Pokemon.h"

#include "Utilities/fakeit.hpp"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySwitchHandlerTest, "Project.PokemonUI.PartySwitchHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartySwitchHandlerTest::RunTest(const FString& Parameters) {
	Mock<IPartyScreen> Screen;
	Fake(Method(Screen, BeginSwitch));
	
	Mock<IPokemon> Pokemon1;
	Fake(Dtor(Pokemon1));
	
	Mock<IPokemon> Pokemon2;
	Fake(Dtor(Pokemon2));
	
	TArray<TSharedRef<IPokemon>> Party;
	Party.Emplace(&Pokemon1.get());
	
	auto Handler = MakeUniqueRoot<UPartyMenuHandler>(NewObject<UPartySwitchHandler>());
	bool Passed = TestFalse(TEXT("Command should not be visible!"), Handler->ShouldShow(Screen.get(), Party, 0));
	
	Party.Emplace(&Pokemon2.get());
	Passed &= TestTrue(TEXT("Command should be visible!"), Handler->ShouldShow(Screen.get(), Party, 0));

	Handler->Handle(Screen.get(), Party, 0);
	Passed &= Verify(Method(Screen, BeginSwitch).Using(0));
	
	return Passed;
}
