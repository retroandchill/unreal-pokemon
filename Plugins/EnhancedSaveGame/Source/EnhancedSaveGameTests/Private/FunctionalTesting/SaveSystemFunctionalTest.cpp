// "Unreal Pokémon" created by Retro & Chill.


#include "FunctionalTesting/SaveSystemFunctionalTest.h"
#include "Saving/SaveGameManager/ScopedInMemorySaveHandle.h"


void ASaveSystemFunctionalTest::AddScopedTestObjects() {
    AddScopedData<EnhancedSaveGame::FScopedInMemorySaveHandle>();
}