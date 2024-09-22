// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Lookup/InjectionTarget.h"

#include "DependencyInjectionSettings.generated.h"

/**
 * The settings related to dependency injection related tasks in Pokémon Core.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Dependency Injection")
class UNREALINJECTOR_API UDependencyInjectionSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
#if WITH_METADATA
    /**
     * Refresh the default dependencies from the list of classes
     */
    void RefreshDependencies();
#endif

    /**
     * The map of interface types to implementation classes
     */
    UPROPERTY(EditDefaultsOnly, Config, EditFixedSize, Category = DependencyInjection, meta = (EditFixedOrder))
    TArray<FInjectionTarget> TargetInjections;

    UPROPERTY(EditDefaultsOnly, Config, EditFixedSize, Category = DependencyInjection, meta = (EditFixedOrder))
    TArray<FInjectionTarget> BlueprintNodeClasses;

  private:
#ifdef WITH_METADATA
    /**
     * Run a sweep for any new interfaces that exist now and load them into the settings
     * @return If there was a change to the data at all
     */
    bool CheckForNewInjectableInterfaces();
#endif
};
