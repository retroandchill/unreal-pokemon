// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ObtainMethod.h"
#include "UObject/Interface.h"

#include "ObtainedBlock.generated.h"

struct FPokemonDTO;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class UObtainedBlock : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for all information how a Pokémon was obtained.
 */
class POKEMONCORE_API IObtainedBlock {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual TScriptInterface<IObtainedBlock> Initialize(const FPokemonDTO& DTO) = 0;
    
    /**
     * Get the method this Pokémon was obtained by.
     * @return The means the Pokémon was obtained by.
     */
    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    virtual EObtainMethod GetObtainMethod() const = 0;

    /**
     * Get the level the Pokémon was met at
     * @return The level the Pokémon was met at
     */
    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    virtual int32 GetLevelMet() const = 0;

    /**
     * Get the time this Pokémon was received
     * @return When was this Pokémon received
     */
    virtual const FDateTime *GetTimeReceived() const = 0;

    /**
     * Get the location or other means of obtaining that this Pokémon has.
     * @return How this Pokémon was obtained
     */
    virtual TOptional<FText> GetObtainText() = 0;

    /**
     * Get the time this Pokémon hatched
     * @return The date and time the egg hatched.
     */
    virtual const FDateTime *GetTimeHatched() const = 0;

    /**
     * Set the this Pokémon hatched
     * @param DateTime The date and time the egg hatched.
     */
    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    virtual void SetTimeHatched(const FDateTime& DateTime) = 0;

    /**
     * Get the map this Pokémon hatched on.
     * @return The location of hatching.
     */
    virtual TOptional<FText> GetHatchedMap() = 0;

    /**
     * Set the map this Pokémon hatched on.
     * @param MapName The location of hatching.
     */
    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    virtual void SetHatchedMap(const FText& MapName) = 0;
};