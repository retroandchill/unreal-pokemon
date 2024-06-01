// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Lookup/InjectionTarget.h"

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
    explicit UDependencyInjectionSettings(const FObjectInitializer &ObjectInitializer);

    /**
     * Get the target injections for each interface.
     * @return The map of interface types to implementation classes
     */
    const TMap<UClass *, FInjectionTarget> &GetTargetInjections() const;

  private:
#ifdef WITH_METADATA
    /**
     * Run a sweep for any new interfaces that exist now and load them into the settings
     * @return If there was a change to the data at all
     */
    bool CheckForNewInjectableInterfaces();
#endif

    /**
     * The map of interface types to implementation classes
     */
    UPROPERTY(EditAnywhere, Config, EditFixedSize, Category = "Dependency Injection", meta = (ReadOnlyKeys))
    TMap<UClass *, FInjectionTarget> TargetInjections;
};
