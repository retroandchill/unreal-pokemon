// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Environment.generated.h"

/**
 * Represents a battle environment which is used for determining the battle base to display as well as the type of
 * Nature Power
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FEnvironment : public FTableRowBase {
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
     * The name pattern used to retrieve the base graphic from the asset list
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals")
    FString BattleBase;
};
