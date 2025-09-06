// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Utilities/GCPointer.h"
#include "Windowskin/WindowskinFactory.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestWindowskinFactory, "Unit Tests.RPGMenus.TestWindowskinFactory",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestWindowskinFactory::RunTest(const FString &Parameters)
{
    TGCPointer Factory(NewObject<UWindowskinFactory>());
    UE_CHECK_TRUE(Factory->ShouldShowInNewMenu());
    TGCPointer NewWindowskin(Factory->FactoryCreateNew(UWindowskin::StaticClass(), Factory.Get(), FName(), RF_NoFlags,
                                                       Factory.Get(), nullptr));
    UE_CHECK_NOT_NULL(NewWindowskin.Get());
    return true;
}
