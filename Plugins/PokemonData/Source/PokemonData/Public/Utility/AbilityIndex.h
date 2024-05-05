// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilityIndex.generated.h"

using FIntTextPair = TPair<int32, FText>;

/**
 * Thin wrapper around the numeric ability index of the Pokémon.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FAbilityIndex {
    GENERATED_BODY()

    /**
     * The internal index used for this class.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Abilities)
    int32 Index = 0;

#if WITH_EDITORONLY_DATA
    TArray<TSharedPtr<FIntTextPair>> TextOptions;
#endif

    FAbilityIndex();
    
    FAbilityIndex(int32 IndexIn);

    FAbilityIndex& operator=(int32 NewIndex);

#if WITH_EDITORONLY_DATA
    void SetDefaultTextOptions();
    
    void UpdateTextOptions(FName Species);
#endif
};
