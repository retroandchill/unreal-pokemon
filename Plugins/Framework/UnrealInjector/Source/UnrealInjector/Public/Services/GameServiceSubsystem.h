// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Casting/InstanceOf.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Concepts/Interfaces.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Optionals/Value.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "Service.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GameServiceSubsystem.generated.h"

/**
 * Subsystem for creating and populating services in the game context.
 */
UCLASS()
class UNREALINJECTOR_API UGameServiceSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;

    /**
     * Gets an already registered service of the specified type.
     *
     * @tparam T The type of the service to retrieve.
     * @return Pointer to the service of type T.
     * @throws AssertionError if the service of type T is not found.
     */
    template <typename T>
        requires std::derived_from<T, UService>
    T &GetService(const TSubclassOf<T> &ServiceClass = T::StaticClass()) const
    {
        // clang-format off
        return Retro::Optionals::OfNullable(Services.Find(ServiceClass)) |
               Retro::Optionals::Transform([](const TObjectPtr<UService>& P) { return P.Get(); }) |
               Retro::Optionals::Transform(Retro::DynamicCastChecked<T>) |
               Retro::Optionals::Value;
        // clang-format on
    }

    /**
     * Retrieves the specified service instance from the game's service subsystem.
     *
     * This method retrieves a service of type T from the game instance's
     * service subsystem. It checks the validity of both the game instance
     * and the subsystem before fetching the requested service.
     *
     * @tparam T The type of the service to retrieve, must be derived from UService.
     * @param WorldContext The context object from which the game instance is obtained.
     * @param ServiceClass The class used for the service class
     * @return A reference to the requested service.
     * @throws Assertion failure if the game instance or the subsystem is not valid.
     */
    template <typename T>
        requires std::derived_from<T, UService>
    static T &GetService(const UObject *WorldContext, const TSubclassOf<T> &ServiceClass = T::StaticClass())
    {
        auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
        check(IsValid(GameInstance))
        auto Subsystem = GameInstance->GetSubsystem<UGameServiceSubsystem>();
        check(IsValid(Subsystem))
        return Subsystem->GetService<T>(ServiceClass);
    }

    /**
     * Static method to retrieve a service instance. It allows you to get a service without getting a pointer to
     * the Game Service Subsystem.
     *
     * @return A pointer to the service instance.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly,
              meta = (AutoCreateRefTerm = "ServiceClass", WorldContext = WorldContext))
    static UService *StaticGetService(const UObject *WorldContext, const TSubclassOf<UService> &ServiceClass);

    /**
     * Retrieves a list of services of the specified type.
     *
     * @tparam T The type of services to retrieve.
     * @return A list of services that match the specified type.
     */
    template <typename T>
        requires std::derived_from<T, UService> || Retro::UnrealInterface<T>
    auto GetServicesOfType() const
    {
        // clang-format off
        return Services |
            Retro::Ranges::Views::Values |
            Retro::Ranges::Views::Filter(Retro::InstanceOf<T>) |
            Retro::Ranges::Views::Transform(Retro::AsInterface<T>);
        // clang-format on
    }

  private:
    UPROPERTY()
    TMap<TSubclassOf<UService>, TObjectPtr<UService>> Services;
};
