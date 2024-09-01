// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "DataRetrieval/DataTableProxy.h"
#include "Ranges/Views/AnyView.h"
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

USTRUCT(BlueprintType, meta = (StructType = Status))
struct POKEMONDATA_API FStatusHandle {
    GENERATED_BODY()

    using FValueType = FStatus;

    FStatusHandle() = default;

    explicit(false) FStatusHandle(FName RowID);

    template <typename... A>
    explicit(false) FStatusHandle(A&&... Args) : RowID(Forward<A>(Args)...) {
    }

    operator FName() const;

private:
    /**
     * The ID of the row in question.
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.StatusEffectHelper.GetStatusEffectNames"))
    FName RowID;

    friend class FPokemonEditorToolsModule;
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
    static FName ConvertStatusHandleToName(const FStatusHandle& Struct);

    UFUNCTION(BlueprintPure, Category = Name,
        meta = (DisplayName = "Convert To StatusHandle", CompactNodeTitle = "->", BlueprintAutocast))
    static FStatusHandle ConvertNameToStatusHandle(FName Name);
};

static_assert(UE::Ranges::UEStruct<FStatus>);
static_assert(Pokemon::Data::DataStruct<FStatus>);
static_assert(Pokemon::Data::DataStructHandle<FStatusHandle>);