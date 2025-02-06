// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataRetrieval/DataTableProxy.h"
#include "IndexedTableRow.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"

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

USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FStatusHandle {
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FStatusHandle, FStatus)

    /**
     * The ID of the row in question.
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.StatusEffectHelper.GetStatusEffectNames"))
    FName RowID;
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

    UFUNCTION(BlueprintPure, Category = StatusHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertStatusHandleToName(const FStatusHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To StatusHandle", CompactNodeTitle = "->", BlueprintAutocast))
    static FStatusHandle ConvertNameToStatusHandle(FName Name);
};

static_assert(Retro::UEStruct<FStatus>);
static_assert(Pokemon::Data::DataStruct<FStatus>);
static_assert(Pokemon::Data::DataStructHandle<FStatusHandle>);