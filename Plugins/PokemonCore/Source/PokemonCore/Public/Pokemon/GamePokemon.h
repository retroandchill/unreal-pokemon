// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "PokemonDTO.h"
#include "Stats/StatBlock.h"
#include "Trainers/OwnerInfo.h"

#include "GamePokemon.generated.h"

class IMoveBlock;
struct FPokemonDTO;
class IMove;

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
UCLASS()
class POKEMONCORE_API UGamePokemon : public UObject, public IPokemon {
    GENERATED_BODY()

  public:
    void Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer) override;

    UFUNCTION(BlueprintPure, Category = Bio)
    FText GetNickname() const override;

    UFUNCTION(BlueprintPure, Category = Bio)
    const FSpeciesData &GetSpecies() const override;

    uint32 GetPersonalityValue() const override;

    UFUNCTION(BlueprintPure, Category = Bio)
    EPokemonGender GetGender() const override;

    UFUNCTION(BlueprintPure, Category = Bio)
    FName GetPokeBall() const override;

    UFUNCTION(BlueprintPure, Category = Bio)
    bool IsShiny() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetCurrentHP() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    int32 GetMaxHP() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    bool IsFainted() const override;

    UFUNCTION(BlueprintPure, Category = Stats)
    TScriptInterface<IStatBlock> GetStatBlock() const override;

    UFUNCTION(BlueprintPure, Category = Moves)
    TScriptInterface<IMoveBlock> GetMoveBlock() const override;

    UFUNCTION(BlueprintPure, Category = Abilities)
    TScriptInterface<IAbilityBlock> GetAbility() const override;

    const FItem *GetHoldItem() const override;

    UFUNCTION(BlueprintPure, Category = Trainer)
    const FOwnerInfo &GetOwnerInfo() const override;
    
    UFUNCTION(BlueprintPure, Category = Trainer)
    TScriptInterface<IObtainedBlock> GetObtainedInformation() const override;

    /**
     * Create a new Pokémon from the given input data
     * @param Data The data to input to create the Pokémon
     * @return The created Pokémon
     */
    static UGamePokemon *Create(const FPokemonDTO &Data);

    /**
     * Create a new Pokémon from the given input data
     * @param Data The data to input to create the Pokémon
     * @return The created Pokémon
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Create New Pokémon", Category = "Objects|Construction")
    static UGamePokemon *Create(const FPokemonDTO &Data, const TScriptInterface<ITrainer> &Trainer);

private:
    /**
     * The ID of the species of Pokémon this is
     */
    UPROPERTY(SaveGame)
    FName Species;

    /**
     * The internal personality value of the Pokémon. Determines the default values of various aspects of the
     * Pokémon if the values are not already set.
     */
    UPROPERTY(SaveGame)
    uint32 PersonalityValue;

    /**
     * The nickname assigned to the Pokémon. Uses the species name if empty.
     */
    UPROPERTY(SaveGame)
    TOptional<FText> Nickname;

    /**
     * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
     */
    UPROPERTY(SaveGame)
    TOptional<EPokemonGender> Gender;

    /**
     * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
     */
    UPROPERTY(SaveGame)
    TOptional<bool> Shiny;

    UPROPERTY(SaveGame)
    FName PokeBall;

    /**
     * The current amount of HP this Pokémon has
     */
    UPROPERTY(SaveGame)
    int32 CurrentHP;

    /**
     * The handler for calculating stats
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IStatBlock> StatBlock;

    /**
     * The block that contains the move data
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IMoveBlock> MoveBlock;

    /**
     * The block that contains the move data
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IAbilityBlock> AbilityBlock;

    /**
     * The hold item for this Pokémon.
     */
    UPROPERTY(SaveGame)
    TOptional<FName> HoldItem;

    /**
     * The information about the original trainer
     */
    UPROPERTY(SaveGame)
    FOwnerInfo OwnerInfo;

    /**
     * The block that contains the obtained information
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IObtainedBlock> ObtainedBlock;
};
