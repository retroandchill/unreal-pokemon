// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "EvolutionMethod.h"
#include "LevelUpMove.h"
#include "SpeciesData.generated.h"
#include "UObject/Object.h"

struct FGenderRatio;
/**
 * Represents an individual species object
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
struct POKEMONDATA_API FSpeciesData : public FTableRowBase {
    GENERATED_BODY()

    /**
     * The internal ID used for lookup by the game
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FName ID;

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The name of this particular form that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText FormName;

    /**
     * The types this Pokémon can have
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Types;

    /**
     * Determines a Pokémon's chance of being Male/Female
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information",
              meta = (GetOptions = "PokemonData.GenderRatioHelper.GetGenderRatioNames"))
    FName GenderRatio;

    /**
     * The catch rate of this species
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
    uint8 CatchRate;

    /**
     * The base happiness of this species when they are caught
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
    uint8 Happiness;

    /**
     * The base stats of this particular species
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames", UIMin = 1, ClampMin = 1))
    TMap<FName, int32> BaseStats;

    /**
     * The rate at which this Pokémon levels up from gaining Exp.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats",
              meta = (GetOptions = "PokemonData.GrowthRateHelper.GetGrowthRateNames"))
    FName GrowthRate;

    /**
     * The base amount of Exp this species gives when defeated/caught
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    int32 BaseExp;

    /**
     * The EVs this species gives when defeated/caught
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames", UIMin = 1, ClampMin = 1))
    TMap<FName, uint8> EVs;

    /**
     * The normal abilities this Pokémon can have
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities",
              meta = (GetOptions = "PokemonData.AbilityHelper.GetAbilityNames"))
    TArray<FName> Abilities;

    /**
     * The hidden abilities this Pokémon can have
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities",
              meta = (GetOptions = "PokemonData.AbilityHelper.GetAbilityNames"))
    TArray<FName> HiddenAbilities;

    /**
     * Moves learned up leveling up
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
    TArray<FLevelUpMove> Moves;

    /**
     * Moves that can be learned TM/HM/TR or through a Move Tutor
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves",
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    TArray<FName> TutorMoves;

    /**
     * Moves that can be learned through breeding or by means of a Mirror Herb
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves",
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    TArray<FName> EggMoves;

    /**
     * Methods of evolution that are possible for this species
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Evolution")
    TArray<FEvolutionMethod> Evolutions;

    /**
     * What breeding egg groups is this Pokémon a part of
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding",
              meta = (GetOptions = "PokemonData.EggGroupHelper.GetEggGroupNames"))
    TArray<FName> EggGroups;

    /**
     * On average how many steps does this Pokémon take to hatch?
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding", meta = (UIMin = 1, ClampMin = 1))
    int32 HatchSteps = 1;

    /**
     * Any Incense items used for breeding?
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding",
              meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    FName Incense;

    /**
     * The species can this Pokémon produce?
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding",
              meta = (GetOptions = "PokemonData.SpeciesHelper.GetSpeciesNames"))
    TArray<FName> Offspring;

    /**
     * The category as displayed to the player on the Pokédex screen
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
    FText Category;

    /**
     * The description for this species as displayed in the Pokédex
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
    FText PokedexEntry;

    /**
     * This Pokémon's height in meters (m)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex", meta = (UIMin = 0.1f, ClampMin = 0.1f))
    float Height = 0.1f;

    /**
     * This Pokémon's weight in kilograms (kg)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex", meta = (UIMin = 0.1f, ClampMin = 0.1f))
    float Weight = 0.1f;

    /**
     * The color of this Pokémon (used for Pokédex filtering)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex",
              meta = (GetOptions = "PokemonData.BodyColorHelper.GetBodyColorNames"))
    FName Color;

    /**
     * The body shape of this Pokémon (used for Pokédex filtering)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex",
              meta = (GetOptions = "PokemonData.BodyShapeHelper.GetBodyShapeNames"))
    FName Shape;

    /**
     * The habitat of this Pokémon (used for Pokédex filtering)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex",
              meta = (GetOptions = "PokemonData.HabitatHelper.GetHabitatNames"))
    FName Habitat;

    /**
     * Item(s) held 100% of the time
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HeldItems",
              meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    TArray<FName> WildItemCommon;

    /**
     * Item(s) held 50% of the time
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HeldItems",
              meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    TArray<FName> WildItemUncommon;

    /**
     * Item(s) held 5% of the time
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "HeldItems",
              meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    TArray<FName> WildItemRare;

    /**
     * The generation this Pokémon was introduced in
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata", meta = (UIMin = 1, ClampMin = 1))
    int32 Generation = 1;

    /**
     * Words/phrases that can be used to group the data"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    TArray<FName> Tags;

    /**
     * Get the Pokémon's gender ratio data
     * @return The gender ratio data of the Pokémon in question
     */
    const FGenderRatio &GetGenderRatio() const;
};

/**
 * Blueprint function library for getting species data out.
 */
UCLASS()
class POKEMONDATA_API USpeciesHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible species names.
     * @return The list of all possible species names.
     */
    UFUNCTION()
    static TArray<FName> GetSpeciesNames();
};