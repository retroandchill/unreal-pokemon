// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Utils/Unreachable.h"

#include "ScopedTestComponent.generated.h"

class AScopedFunctionalTest;

/**
 * @brief An abstract component class used for functional testing in Unreal Engine.
 *
 * UScopedTestComponent is intended to be used as a base class for creating components that can be added
 * to scoped functional tests for the purpose of conducting various tests within the Unreal Engine framework.
 *
 * This class is abstract and requires the implementation of the AddScopedComponent method in derived classes.
 */
UCLASS(Abstract, ClassGroup = (FunctionalTesting), meta = (BlueprintSpawnableComponent))
class RETROLIBTESTS_API UScopedTestComponent : public UActorComponent {
    GENERATED_BODY()

  public:
    /**
     * @brief Adds a scoped component to a functional test.
     *
     * This pure virtual method must be implemented by derived classes to add themselves
     * to a given scoped functional test. This is essential for running the test within
     * the Unreal Engine framework.
     *
     * @param TestObject The scoped functional test to which the component should be added.
     */
    virtual void AddScopedComponent(AScopedFunctionalTest &TestObject) ABSTRACT_METHOD;
};