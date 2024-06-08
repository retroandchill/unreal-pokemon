// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Battler.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONBATTLE_API UBattler : public UInterface {
    GENERATED_BODY()
};

/**
 * Wrapper around a Pokémon for its usage in battle
 */
class POKEMONBATTLE_API IBattler {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Get the Pokémon's level
     * @return The level of the Pokémon in question
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetLevel() const = 0;

    /**
     * Get the value of the Pokémon's physical attack.
     * @return The Pokémon's physical attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetAttack() const = 0;

    /**
     * Get the value of the Pokémon's physical defense.
     * @return The Pokémon's physical defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetDefense() const = 0;

    /**
     * Get the value of the Pokémon's special attack.
     * @return The Pokémon's special attack
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetSpecialAttack() const = 0;

    /**
     * Get the value of the Pokémon's special defense.
     * @return The Pokémon's special defense
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetSpecialDefense() const = 0;

};