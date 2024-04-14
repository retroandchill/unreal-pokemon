// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Meta/MetatypeDeclares.h"
#include "Pokemon/Pokemon.h"

#include "StatEntry.generated.h"

struct FStat;
struct FNature;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UStatEntry : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents an individual stat entry on a Pokémon
 */
class POKEMONCORE_API IStatEntry {
    GENERATED_BODY()

  public:
    /**
     * Initialize the stat with the given IV and EV
     * @param Stat The stat in question to set this to
     * @param NewPersonalityValue The personality value of the owning Pokémon
     * @param IVs The IV of the stat
     * @param EVs The EV of the stat
     */
    virtual TScriptInterface<IStatEntry> Initialize(FName Stat, const TOptional<int32> &IVs, int32 EVs = 0) = 0;

    /**
     * Get the value of this given stat in question
     * @return The value of the stat
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetStatValue() const = 0;

    /**
     * Recalculate the value of the stat given the following information
     * @param Level The level of the Pokémon
     * @param Base The base stat of the Pokémon in question
     * @param Nature The Pokémon's effective nature
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual void RefreshValue(int32 Level, int32 Base, const FNature &Nature) = 0;

    /**
     * Get the information about the stat in question
     * @return The stat's specific information
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual const FStat &GetStat() const = 0;

    /**
     * Get the ID of the stat in question
     * @return The ID of the stat in question
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual FName GetID() const = 0;

    /**
     * Get the IV of the stat
     * @return The value of the Pokémon's IV
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetIV() const = 0;

    /**
     * Get the EV of the stat
     * @return The value of the Pokémon's EV
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetEV() const = 0;
};
