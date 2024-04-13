// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TrainerGender.h"

#include "TrainerType.generated.h"

/**
 * The data for the TrainerType class
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FTrainerType : public FTableRowBase {
    GENERATED_BODY()

    /**
     * ID of this trainer type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FName ID;

    /**
     * Name of this trainer type as displayed by the game.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The gender of this particular trainer type
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    ETrainerGender Gender;

    /**
     * The amount of money payed out by this trainer. Multiplied by their level of the last Pokémon in the party.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Payout", meta = (UIMin = 0, ClampMin = 0))
    int32 BaseMoney;

    /**
     * Tells the AI how skilled to make this trainer
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI", meta = (UIMin = 0, ClampMin = 0))
    int32 SkillLevel;

    /**
     * Words/phrases that can be used to group certain kinds of trainer types.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Meta")
    TArray<FName> Tags;

    /**
     * The music played when the trainer spots the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Music")
    TObjectPtr<USoundBase> IntroMusic;

    /**
     * The music played when battling the trainer
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Music")
    TObjectPtr<USoundBase> BattleMusic;

    /**
     * The music played when winning against the trainer
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Music")
    TObjectPtr<USoundBase> VictoryMusic;
};
