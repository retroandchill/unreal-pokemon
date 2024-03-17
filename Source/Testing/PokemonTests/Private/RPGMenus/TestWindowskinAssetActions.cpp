// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Data/Windowskin.h"
#include "Misc/AutomationTest.h"
#include "Windowskin/WindowskinAssetActions.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestWindowskinAssetActions, "Project.RPGMenus.TestWindowskinAssetActions",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestWindowskinAssetActions::RunTest(const FString& Parameters) {
	FWindowskinAssetActions  AssetActions;

	AUTOMATION_ASSERT(TestEqual(TEXT("Name"), AssetActions.GetName().ToString(), TEXT("Windowskin")));
	AUTOMATION_ASSERT(TestEqual(TEXT("Asset Class"), AssetActions.GetSupportedClass(), UWindowskin::StaticClass()));
	AUTOMATION_ASSERT(TestEqual(TEXT("Type Color"), AssetActions.GetTypeColor(), FColor::Silver));
	AUTOMATION_ASSERT(TestEqual(TEXT("Categories"), AssetActions.GetCategories(), EAssetTypeCategories::UI));
	
	return true;
}
