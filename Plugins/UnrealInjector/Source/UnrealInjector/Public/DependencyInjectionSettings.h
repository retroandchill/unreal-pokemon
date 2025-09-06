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
class UNREALINJECTOR_API UDependencyInjectionSettings : public UDeveloperSettings
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditDefaultsOnly, Config, Category = DependencyInjection, meta = (ContentDir))
    TArray<FDirectoryPath> BlueprintServiceScan;

    /**
     * The map of interface types to implementation classes
     */
    UPROPERTY(EditDefaultsOnly, Config, EditFixedSize, Category = DependencyInjection, meta = (EditFixedOrder))
    TArray<FInjectionTarget> TargetInjections;
};
