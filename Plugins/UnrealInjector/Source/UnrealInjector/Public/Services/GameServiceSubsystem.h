// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameServiceSubsystem.generated.h"

class UService;

/**
 * Subsystem for creating and populating services in the game context.
 */
UCLASS()
class UNREALINJECTOR_API UGameServiceSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& Collection) override;
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
    T* GetService() {
        return CastChecked<T>(Services.FindChecked(T::StaticClass()));
    }

    /**
     * Retrieves a registered service of the specified class type from the game service subsystem.
     *
     * @param ServiceClass The TSubclassOf<UService> representing the service class type to retrieve.
     * @return A pointer to the registered service if found; otherwise, returns nullptr.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly,
        meta = (AutoCreateRefTerm = "ServiceClass", DeterminesOutputType = "ServiceClass"))
    UService* GetService(UPARAM(meta = (AllowAbstract = "false")) const TSubclassOf<UService> &ServiceClass);

private:
    UPROPERTY()
    TMap<TSubclassOf<UService>, TObjectPtr<UService>> Services;

};
