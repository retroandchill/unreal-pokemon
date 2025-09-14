// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTesting/SaveSystemTestComponent.h"
#include "FunctionalTesting/ScopedFunctionalTest.h"
#include "Saving/SaveGameManager/ScopedInMemorySaveHandle.h"

void USaveSystemTestComponent::AddScopedComponent(AScopedFunctionalTest &TestObject)
{
    TestObject.AddScopedData<EnhancedSaveGame::FScopedInMemorySaveHandle>();
}