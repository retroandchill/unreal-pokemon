// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "EncounterCategory.h"
#include "EncounterType.generated.h"
#include "UObject/Object.h"

/**
 * Represents any of the various encounter types that may occur within the world
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FEncounterType : public FTableRowBase {
    GENERATED_BODY()

    /**
     * The internal ID used for lookup by the game
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FName ID;

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The category of encounter this belongs to
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Encounter Info")
    EEncounterCategory Type;

    /**
     * The weight applied for when an encounter will occur when taking a step
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Encounter Info",
              meta = (UIMin = 0, ClampMin = 10, UIMax = 100, ClampMax = 100))
    int32 TriggerChance;
};
