// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "ConstructionUtilities.generated.h"

class ITrainer;
class IAbilityBlock;
class IMoveBlock;
class IStatBlock;
struct FPokemonDTO;
class IPokemon;

/**
 * Utility class used for constructing game objects, with specific arguments.
 */
UCLASS()
class POKEMONCORE_API UConstructionUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Create a new Pokémon from the provided data.
     * @param Data The data used to construct the Pokémon.
     * @return The created Pokémon.
     */
    UFUNCTION(BlueprintCallable, Category = "Object|Constructors")
    static TScriptInterface<IPokemon> CreateNewPokemon(const FPokemonDTO &Data);

    /**
     * Create a new Pokémon from the provided data and a foriegn trianer ID.
     * @param Data The data used to construct the Pokémon.
     * @param Trainer The original trainer used to construct this trainer
     * @return The created Pokémon.
     */
    UFUNCTION(BlueprintCallable, Category = "Object|Constructors")
    static TScriptInterface<IPokemon> CreateForeignPokemon(const FPokemonDTO &Data, const TScriptInterface<ITrainer>& Trainer);

    /**
     * Create the stat block based on the settings.
     * @param Owner The Pokémon to take ownership of the stat block
     * @param DTO The DTO to copy any saved data from
     * @return The created block
     */
    static TScriptInterface<IStatBlock> CreateStatBlock(const TScriptInterface<IPokemon> &Owner,
                                                        const FPokemonDTO &DTO);

    /**
     * Create the move block based on the settings.
     * @param Owner The Pokémon to take ownership of the move block
     * @param DTO The DTO to copy any saved data from
     * @return The created block
     */
    static TScriptInterface<IMoveBlock> CreateMoveBlock(const TScriptInterface<IPokemon> &Owner,
                                                        const FPokemonDTO &DTO);

    /**
     * Create the ability block based on the settings.
     * @param Owner The Pokémon to take ownership of the ability block
     * @param DTO The DTO to copy any saved data from
     * @return The created block
     */
    static TScriptInterface<IAbilityBlock> CreateAbilityBlock(const TScriptInterface<IPokemon> &Owner,
                                                              const FPokemonDTO &DTO);
};
