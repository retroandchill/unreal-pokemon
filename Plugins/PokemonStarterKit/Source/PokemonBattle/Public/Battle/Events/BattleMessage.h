// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleMessage.generated.h"

/**
 * Struct that contains a message as well as optional visual sequence that can be played.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleMessage {
    GENERATED_BODY()

    /**
     * The message to display
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Messages")
    FText Message;

    /**
     * Default constructor
     */
    FBattleMessage() = default;

    /**
     * Construct a new battle message with the given message content and no animation
     * @param Message The message to display
     */
    explicit FBattleMessage(FText &&Message);
};
