// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "DependencyInjectionSettings.generated.h"

/**
 * The settings related to dependency injection related tasks in Pokémon Core.
 */
UCLASS(Config = Game, DefaultConfig)
class UNREALINJECTOR_API UDependencyInjectionSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Creat the default version of this object.
     * @param ObjectInitializer The initializer for this object
     */
    explicit UDependencyInjectionSettings(const FObjectInitializer& ObjectInitializer);

    /**
     * Get the target injections for each interface.
     * @return The map of interface types to implementation classes
     */
    const TMap<UClass*, TSubclassOf<UObject>>& GetTargetInjections() const;
    

  private:
    /**
     * The map of interface types to implementation classes
     */
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    TMap<UClass*, TSubclassOf<UObject>> TargetInjections; 
    
};
