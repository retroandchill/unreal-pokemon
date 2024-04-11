// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Type.generated.h"

/**
 * Represents the types that belong to both individual species as well as the moves used by them
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
struct POKEMONDATA_API FType : public FTableRowBase {
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
     * Graphic that represents a sprite in the menu
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals")
    UPaperSprite *Icon;

    /**
     * Is this type special? (Only means anything when the Physical/Special-split is disabled)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Type Info")
    bool IsSpecialType = false;

    /**
     * Is this type only a type used by moves amd not one that a Pokémon can be.
     *
     * Examples of this would include the "???" Type used by Curse in generation II-IV and Shadow Moves.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Type Info")
    bool IsPseudoType = false;

    /**
     * Move types that deal super-effective damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Weaknesses;

    /**
     * Move types that deal not very effective damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Resistances;

    /**
     * Move types that deal no damage to this type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Matchups",
              meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TArray<FName> Immunities;

    /**
     * Words/phrases that can be used to group certain kinds of types"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    TArray<FName> Tags;
};

/**
 * Blueprint function library for getting type data out.
 */
UCLASS()
class POKEMONDATA_API UTypeHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible type names.
     * @return The list of all possible type names.
     */
    UFUNCTION()
    static TArray<FName> GetTypeNames();
};
