// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Pokemon.h"
#include "UObject/Interface.h"

#include "PokemonUtilities.generated.h"

/**
 * The delegate called when the level up procedure end
 */
DECLARE_DYNAMIC_DELEGATE(FLevelUpEnd);

/**
 * The delegate called when the move learn procedure ends
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FMoveLearnEnd, bool, bMoveLearned);

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

    /**
     * The changes to the individual stats in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    TMap<FName, FStatChange> StatChanges;
};

// This class does not need to be modified.
UINTERFACE()
class UPokemonUtilities : public UInterface {
    GENERATED_BODY()
};

/**
 * Utility interface for Pokémon related operations
 */
class POKEMONCORE_API IPokemonUtilities {
    GENERATED_BODY()

  public:
    /**
     * Called after a Pokémon level's up
     * @param WorldContext The world context object
     * @param Pokemon The Pokémon that leveled up
     * @param StatChanges The changes to the stats in battle
     * @param bShowMessage Should the level up message be shown?
     * @param OnEnd Called when the level up process ends
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Utilities|Pokémon")
    void ProcessLevelUp(const UObject *WorldContext, const TScriptInterface<IPokemon> &Pokemon,
                        const FLevelUpStatChanges &StatChanges, bool bShowMessage, const FLevelUpEnd &OnEnd);

    /**
     * Teach a move to a Pokémon
     * @param WorldContext The object used to obtain the world context information
     * @param Pokemon The Pokémon to learn the move
     * @param Move The move to learn
     * @param OnEnd What is called after all messages are displayed
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Utilities|Pokémon")
    void LearnMove(const UObject *WorldContext, const TScriptInterface<IPokemon> &Pokemon, FName Move,
                   const FMoveLearnEnd &OnEnd);
};