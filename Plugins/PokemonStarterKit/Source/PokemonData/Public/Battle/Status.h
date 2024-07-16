// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "UObject/Object.h"

#include "Status.generated.h"

/**
 * Represents the data for a status condition
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FStatus : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The animation displayed when this status is inflicted and when its effect goes off
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals", meta = (MustImplement = "AnimationBase"))
    TSubclassOf<AActor> Animation;
};

/**
 * Helper functions for status effects
 */
UCLASS()
class POKEMONDATA_API UStatusEffectHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the names of all status effects
     * @return The array of all inflictable status effects
     */
    UFUNCTION()
    static TArray<FName> GetStatusEffectNames();
};