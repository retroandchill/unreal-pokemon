// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatBlock.h"
#include "DefaultStatBlock.generated.h"

struct FStatBlockDTO;

/**
 * The default stat calculator, which uses the formulas from the main series games
 */
UCLASS()
class POKEMONCORE_API UDefaultStatBlock : public UObject, public IStatBlock {
    GENERATED_BODY()

  public:
    void Initialize(const ::TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetLevel() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetExp() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetExpForNextLevel() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    const FNature &GetNature() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    TScriptInterface<IStatEntry> GetStat(FName Stat) const override;

    void ForEachStat(TFunctionRef<void(FName, const IStatEntry &)> Predicate) const override;

    UFUNCTION(BlueprintCallable, Category = Stats)
    void CalculateStats(const TMap<FName, int32> &BaseStats) override;

  private:
    /**
     * The Pokémon that owns this object
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IPokemon> Owner;

    /**
     * The level of the Pokémon
     */
    UPROPERTY(SaveGame)
    int32 Level;

    /**
     * The current Exp of this Pokémon
     */
    UPROPERTY(SaveGame)
    int32 Exp;

    /**
     * The Pokémon's Nature ID
     */
    UPROPERTY(SaveGame)
    TOptional<FName> Nature;

    /**
     * Map to each of the Pokémon's individual stat values
     */
    UPROPERTY(SaveGame)
    TMap<FName, TScriptInterface<IStatEntry>> Stats;
};
