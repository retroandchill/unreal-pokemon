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
    explicit UDependencyInjectionSettings(const FObjectInitializer& ObjectInitializer);

    const TMap<UClass*, TSubclassOf<UObject>>& GetTargetInjections() const;
    

  private:
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    TMap<UClass*, TSubclassOf<UObject>> TargetInjections; 
    
};
