// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EditorFunctionalTest.h"
#include "Utilities/ScopedFunctionalTest.h"

#include "SaveSystemFunctionalTest.generated.h"

/**
 * @brief Derived test class for functional testing of the save system.
 *
 * Inherits from AScopedFunctionalTest and is specifically designed to test
 * the enhanced save game functionalities. This class overrides the
 * AddScopedTestObjects method to add objects that are in scope for the test.
 */
UCLASS()
class ENHANCEDSAVEGAMETESTS_API ASaveSystemFunctionalTest : public AScopedFunctionalTest {
    GENERATED_BODY()

protected:
    void AddScopedTestObjects() override;
    

};