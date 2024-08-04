// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "EncounterCategory.h"
#include "IndexedTableRow.h"
#include "UObject/Object.h"

#include "EncounterType.generated.h"

/**
 * Represents any of the various encounter types that may occur within the world
 * @deprecated The encounter system uses a gameplay tag based system now and no longer needs this
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FEncounterType : public FIndexedTableRow {
    GENERATED_BODY()

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
              meta = (UIMin = 1, ClampMin = 1, UIMax = 100, ClampMax = 100))
    int32 TriggerChance;
};