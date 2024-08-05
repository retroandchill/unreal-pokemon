﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Breeding/PokemonGender.h"
#include "TrainerMemo/ObtainMethod.h"
#include "Trainers/OwnerInfo.h"

#include "PokemonDTO.generated.h"

USTRUCT(BlueprintType)
struct POKEMONCORE_API FMoveDTO {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data",
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    FName Move;

    UPROPERTY(EditAnywhere, Category = Data)
    TOptional<int32> CurrentPP;
};

/**
 * Data transfer object for a Pokémon. This is what gets serialized for both the builder and for any communication
 * over the network.
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FPokemonDTO {
    GENERATED_BODY()

    /**
     * The ID of the species this Pokémon represents
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data",
              meta = (GetOptions = "PokemonData.SpeciesHelper.GetSpeciesNames"))
    FName Species;

    /**
     * The level of the created Pokémon
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data, meta = (UIMin = 1, ClampMin = 1))
    int32 Level = 5;

    /**
     * The internal personality value of the Pokémon. Determines the default values of various aspects of the
     * Pokémon if the values are not already set.
     */
    UPROPERTY(EditAnywhere, Category = Data)
    TOptional<uint32> PersonalityValue;

    /**
     * The nickname assigned to the Pokémon. Uses the species name if empty.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TOptional<FText> Nickname;

    /**
     * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TOptional<EPokemonGender> Gender;

    /**
     * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TOptional<bool> Shiny;

    /**
     * The Poké Ball the Pokémon was caught in.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.ItemHelper.GetPokeBallNames"))
    TOptional<FName> PokeBall;

    /**
     * The current amount of HP this Pokémon has
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (UIMin = 0, ClampMin = 0))
    TOptional<int32> CurrentHP;

    /**
     * The amount of Exp the Pokémon has
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<int32> Exp;

    /**
     * The IVs of the Pokémon to explicitly set
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames", UIMin = 0, ClampMin = 0, UIMax = 31,
                      ClampMax = 31))
    TMap<FName, int32> IVs;

    /**
     * The EVs of the Pokémon to explicitly set
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames", UIMin = 0, ClampMin = 0, UIMax = 252,
                      ClampMax = 252))
    TMap<FName, int32> EVs;

    /**
     * The Nature of the Pokémon in question
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.NatureHelper.GetNatureNames"))
    TOptional<FName> Nature;

    /**
     * The ability of the Pokémon
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.AbilityHelper.GetAbilityNames"))
    TOptional<FName> Ability;

    /**
     * The item held by the Pokémon
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    TOptional<FName> Item;

    /**
     * The moves known by the Pokémon
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    TArray<FMoveDTO> Moves;

    /**
     * The moves known by the Pokémon
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data,
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    TSet<FName> MoveMemory;

    /**
     * The method the Pokémon was obtained with
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    EObtainMethod ObtainMethod = EObtainMethod::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<int32> LevelMet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<FDateTime> TimeReceived;

    /**
     * Manual override of the met location (otherwise use the map they were obtained on)
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<FText> MetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<FDateTime> TimeHatched;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<FText> HatchedMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Data)
    TOptional<FOwnerInfo> OwnerInfo;
};
