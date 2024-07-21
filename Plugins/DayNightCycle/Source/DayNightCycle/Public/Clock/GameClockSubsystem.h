// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameClockSubsystem.generated.h"

class IGameClock;
/**
 * Subsystem used for managing the game's clock and emitting events related to the time change.
 */
UCLASS()
class DAYNIGHTCYCLE_API UGameClockSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;
    
    /**
     * Get the clock interface object
     * @return The clock object used to handle the current time
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Clock)
    const TScriptInterface<IGameClock>& GetGameClock() const;
    
private:
    /**
     * The clock object used to handle the current time
     */
    UPROPERTY(BlueprintGetter = GetGameClock, Category = Clock)
    TScriptInterface<IGameClock> GameClock;

};
