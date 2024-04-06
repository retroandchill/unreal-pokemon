// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Memory/GCPointer.h"
#include "Misc/AutomationTest.h"
#include "Windowskin/WindowskinFactory.h"

class UWindowskinFactory;
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestWindowskinFactory, "UnrealPokemon.RPGMenus.TestWindowskinFactory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestWindowskinFactory::RunTest(const FString& Parameters) {

	TGCPointer Factory(NewObject<UWindowskinFactory>());

	AUTOMATION_ASSERT(TestTrue(TEXT("Show in menu"), Factory->ShouldShowInNewMenu()));

	TGCPointer NewWindowskin(Factory->FactoryCreateNew(UWindowskin::StaticClass(), Factory.Get(), FName(), RF_NoFlags, Factory.Get(), nullptr));
	AUTOMATION_ASSERT(TestTrue(TEXT("Show in menu"), NewWindowskin != nullptr));
	
	return true;
}
