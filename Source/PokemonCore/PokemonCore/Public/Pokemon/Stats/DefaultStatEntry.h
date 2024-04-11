// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "StatEntry.h"
#include "DefaultStatEntry.generated.h"

class IPokemon;
/**
 * The default setup of a Pokémon's stat calculation as defined in the main series games
 */
UCLASS(Abstract)
class POKEMONCORE_API UDefaultStatEntry : public UObject, public IStatEntry {
    GENERATED_BODY()

  public:
    TScriptInterface<IStatEntry> Initialize(FName Stat, const TOptional<int32> &IVs, int32 EVs) override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetStatValue() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    const FStat &GetStat() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    FName GetStatID() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetIV() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetEV() const override;

    UFUNCTION(BlueprintCallable, Category = Stats)
    void RefreshValue(int32 Level, int32 Base, const FNature &Nature) override;

  protected:
    /**
     * Set the value of the stat in question
     * @param NewValue The value of the stat
     */
    void SetStatValue(int32 NewValue);

  private:
    /**
     * The ID of the stat in question
     */
    UPROPERTY(SaveGame)
    FName StatID;

    /**
     * The value of the stat's IV
     */
    UPROPERTY(SaveGame)
    int32 IV;

    /**
     * The value of the stat's EV
     */
    UPROPERTY(SaveGame)
    int32 EV;

    /**
     * The actual value of the stat in question
     */
    UPROPERTY(SaveGame)
    int32 Value = 0;
};
