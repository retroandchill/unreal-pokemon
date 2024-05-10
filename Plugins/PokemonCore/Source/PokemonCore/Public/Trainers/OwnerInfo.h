// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Trainers/TrainerGender.h"

#include "OwnerInfo.generated.h"

class ITrainer;
/**
 * The owner information for a given Pokémon
 */
USTRUCT(BlueprintType)
struct  POKEMONCORE_API FOwnerInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokémon|Original Trainer")
    FText OriginalTrainerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokémon|Original Trainer")
    ETrainerGender OriginalTrainerGender = ETrainerGender::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokémon|Original Trainer")
    int32 ID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pokémon|Original Trainer")
    int32 SecretID = 0;

    FOwnerInfo();

    explicit FOwnerInfo(const ITrainer& Trainer);
};
