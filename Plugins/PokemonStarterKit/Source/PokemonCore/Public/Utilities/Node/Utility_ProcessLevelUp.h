// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprints/BlueprintUtilityNode.h"
#include "Lookup/InjectableDependency.h"

#include "Utility_ProcessLevelUp.generated.h"

class IPokemon;
/**
 * The change to a single stat for a Pokémon
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FStatChange {
    GENERATED_BODY()

    /**
     * The stat value before the level up
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32 Before;

    /**
     * The stat value after the level up
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32 After;

    int32 Diff() const;
};

/**
 * The change to the Exp. bar fill for a Pokémon
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FExpPercentChange {
    GENERATED_BODY()

    /**
     * The stat value before the level up
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Before;

    /**
     * The stat value after the level up
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float After;
};

/**
 * The changes to a stat following a level up
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FLevelUpStatChanges {
    GENERATED_BODY()

    /**
     * The value of the level
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    FStatChange LevelChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    FExpPercentChange ExpPercentChange;

    /**
     * The changes to the individual stats in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    TMap<FName, FStatChange> StatChanges;

    FLevelUpStatChanges &operator+=(const FLevelUpStatChanges &Other);
};

/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class POKEMONCORE_API UUtility_ProcessLevelUp : public UBlueprintUtilityNode {
    GENERATED_BODY()

  public:
    void Execute(const TScriptInterface<IPokemon> &Pokemon, const FLevelUpStatChanges &StatChanges, bool bShowMessage,
                 FSimpleDelegate &&OnEnd);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Execute(const TScriptInterface<IPokemon> &Pokemon, const FLevelUpStatChanges &StatChanges, bool bShowMessage);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteOnEnd();

  private:
    FSimpleMulticastDelegate EndProcess;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, UUtility_ProcessLevelUp);
