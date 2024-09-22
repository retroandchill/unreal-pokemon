// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "InjectionTarget.generated.h"

/**
 * Wrapper struct for an injection target. Used for details panel customization.
 */
USTRUCT()
struct UNREALINJECTOR_API FInjectionTarget {
    GENERATED_BODY()

    /**
     * The target interface, used to constrain the type of the injected class
     */
    UPROPERTY(VisibleAnywhere, Category = "Dependency Injection")
    FSoftClassPath TargetInterface;

    /**
     * The actual class to be injected.
     */
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    FSoftClassPath InjectedClass;

    /**
     * Default constructor.
     */
    FInjectionTarget();

    /**
     * Construct a new instance with the specified interface
     * @param InterfaceType The interface type to use
     */
    explicit FInjectionTarget(UClass* InterfaceType);
};
