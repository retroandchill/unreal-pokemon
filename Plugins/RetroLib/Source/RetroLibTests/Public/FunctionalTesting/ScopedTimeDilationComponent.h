// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ScopedTestComponent.h"
#include "ScopedTimeDilationComponent.generated.h"

/**
 * @brief A component that applies a scoped time dilation effect to a functional test.
 *
 * UScopedTimeDilationComponent is responsible for adjusting the time dilation factor during
 * functional testing scenarios. It inherits from UScopedTestComponent and implements
 * the AddScopedComponent method to integrate with the scoped functional test framework.
 *
 * @ingroup FunctionalTesting
 * @blueprintspawnable
 */
UCLASS(ClassGroup = (FunctionalTesting), meta = (BlueprintSpawnableComponent))
class RETROLIBTESTS_API UScopedTimeDilationComponent : public UScopedTestComponent {
    GENERATED_BODY()

  public:
    void AddScopedComponent(AScopedFunctionalTest &TestObject) override;

  private:
    /**
     * @brief Factor by which time is dilated.
     *
     * This variable scales the passage of time. A value of 1.0 means time progresses normally.
     * Lower values slow down time, while higher values speed it up. The minimum allowable value
     * is 0.0001f, as enforced by both the UI and runtime clamping. This allows fine control
     * over time within the application, useful for scenarios such as slow-motion effects
     * or game mechanics that require temporal modulation.
     *
     * @editcategory TimeDilation
     * @edittype float
     * @editmeta UIMin = 0.0001f, ClampMin = 0.0001f
     */
    UPROPERTY(EditAnywhere, Category = TimeDilation, meta = (UIMin = 0.0001f, ClampMin = 0.0001f))
    float TimeDilationFactor = 1.0f;
};