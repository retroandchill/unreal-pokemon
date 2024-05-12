// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TrainerSettings.generated.h"

/**
 * Settings related to trainers.
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONCORE_API UTrainerSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     * @return The maximum number of Pokémon a trainer can carry at any given time
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
    int32 GetMaxPartySize() const;

    

private:
    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxPartySize, Config, Category = Party,
              meta = (UIMin = 1, ClampMin = 1))
    int32 MaxPartySize = 6;

};
