// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTesting/ScopedTestComponent.h"
#include "SaveSystemTestComponent.generated.h"


/**
 * @class USaveSystemTestComponent
 * @brief A component class for handling enhanced save game tests.
 *
 * Inherits from UScopedTestComponent to integrate enhanced save game functionality
 * within the Unreal Engine testing framework.
 */
UCLASS(ClassGroup=(FunctionalTest), meta=(BlueprintSpawnableComponent))
class ENHANCEDSAVEGAMETESTS_API USaveSystemTestComponent : public UScopedTestComponent {
    GENERATED_BODY()

public:
    void AddScopedComponent(AScopedFunctionalTest& TestObject) override;


};