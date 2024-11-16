#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Saving/SaveGameManager/InMemorySaveGameSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestInMemorySaveSystem, "Unit Tests.EnhancedSaveGame.TestInMemorySaveSystem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestInMemorySaveSystem::RunTest(const FString &Parameters) {
    EnhancedSaveGame::FInMemorySaveGameSystem SaveSystem;
    UE_CHECK_FALSE(SaveSystem.PlatformHasNativeUI());
    UE_CHECK_FALSE(SaveSystem.DoesSaveSystemSupportMultipleUsers());

    constexpr auto SaveGameName = TEXT("TestInMemorySaveSystem");
    UE_CHECK_FALSE(SaveSystem.DoesSaveGameExist(SaveGameName, 0));

    TArray<uint8> Data;
    Data.Reserve(100);
    for (uint8 i = 0; i < 100; i++) {
        Data.Emplace(i);
    }
    UE_CHECK_TRUE(SaveSystem.SaveGame(false, SaveGameName, 0, Data));
    Data.Reset();
    UE_CHECK_TRUE(SaveSystem.LoadGame(false, SaveGameName, 0, Data));
    UE_CHECK_EQUAL(100, Data.Num());

    UE_CHECK_FALSE(SaveSystem.DeleteGame(false, SaveGameName, 1));
    UE_CHECK_TRUE(SaveSystem.DeleteGame(false, SaveGameName, 0));

    Data.Reset();
    UE_CHECK_FALSE(SaveSystem.LoadGame(false, SaveGameName, 0, Data));
    
    return true;
}