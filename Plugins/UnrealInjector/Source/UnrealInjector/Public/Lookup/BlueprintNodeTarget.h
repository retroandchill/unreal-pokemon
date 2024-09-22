// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "BlueprintNodeTarget.generated.h"

/**
 * The node target for a Blueprint Utility Node.
 */
USTRUCT()
struct UNREALINJECTOR_API FBlueprintNodeTarget {
    GENERATED_BODY()

    /**
     * The target interface, used to constrain the type of the injected class
     */
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    TSubclassOf<UObject> TargetClass;

    /**
     * The actual class to be injected.
     */
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    FSoftClassPath InjectedClass;

    /**
     * Default constructor.
     */
    FBlueprintNodeTarget();

    /**
     * Construct a new instance with the specified interface
     * @param NodeType The interface type to use
     */
    explicit FBlueprintNodeTarget(const TSubclassOf<UObject> &NodeType);

};
