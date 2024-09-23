// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DependencyInjectionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "DependencyInjectionSubsystem.generated.h"

class UDependencyInjectionSettings;
/**
 *
 */
UCLASS()
class UNREALINJECTOR_API UDependencyInjectionSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    /**
     * Inject the default depene
     * @tparam T The type of the dependency being injected
     * @tparam A The arguments to the Initialize method on the interface
     * @param Outer The UObject that owns the injected dependency
     * @param Args The arguments to forward to the initialize method
     * @return The created interface
     */
    template <typename T, typename... A>
    TScriptInterface<T> InjectDependency(UObject *Outer, A &&...Args) {
        auto &InterfaceClass = TargetInjections.FindChecked(T::UClassType::StaticClass());
        TScriptInterface<T> CreatedInterface = NewObject<UObject>(Outer, InterfaceClass);
        CreatedInterface->Initialize(Forward<A>(Args)...);
        return CreatedInterface;
    }

  private:
    UPROPERTY()
    TMap<TSubclassOf<UInterface>, TSubclassOf<UObject>> TargetInjections;
};
