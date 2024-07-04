// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleMessage.generated.h"

class IBattleAnimation;

/**
 * When in relation to the message should the animation be played
 */
UENUM(BlueprintType)
enum class EAnimationPlacement : uint8 {
    /**
     * Play the animation before the message
     */
    Before,

    /**
     * Play the animation after the message
     */
    After
};

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
     * Optional animation that can be played
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Messages")
    TScriptInterface<IBattleAnimation> Animation;

    /**
     * When should the animation play (if applicable)
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Messages")
    EAnimationPlacement AnimationPlacement = EAnimationPlacement::Before;

    /**
     * Default constructor
     */
    FBattleMessage() = default;

    /**
     * Construct a new battle message with the given message content and no animation
     * @param Message The message to display
     */
    explicit FBattleMessage(FText &&Message);

    /**
     * Construct a new battle message with the given message content and no animation
     * @param Message The message to display
     * @param Animation The animation to play
     * @param AnimationPlacement When in relation should the message should the animation play
     */
    explicit FBattleMessage(FText &&Message, const TScriptInterface<IBattleAnimation>& Animation, EAnimationPlacement AnimationPlacement);
};

/**
 * Struct that wraps around a set of messages that allows for trading off between Blueprints and C++
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FRunningMessageSet {
    GENERATED_BODY()

    /**
     * The containted messages in the set
     */
    TSharedRef<TArray<FBattleMessage>> Messages = MakeShared<TArray<FBattleMessage>>();
    
};

/**
 * Blueprint function library for any functions that are needed while a move is being used.
 */
UCLASS()
class UBattleMessageHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Get the messages from the given message set
     * @param Messages The source message set
     * @return The messages inside the message set
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Messages")
    static const TArray<FBattleMessage> &GetMessages(const FRunningMessageSet &Messages);

    /**
     * Add a new stand-alone message to the list of running messages
     * @param Messages The messages to append
     * @param Message The message to append to the list
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Messages")
    static void AppendMessage(const FRunningMessageSet& Messages, FText Message);

    /**
     * Add a new stand-alone message to the list of running messages
     * @param Messages The messages to append
     * @param Message The message to append to the list
     * @param Animation The animation to play
     * @param AnimationPlacement When in relation should the message should the animation play
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Messages")
    static void AppendMessageWithAnimation(const FRunningMessageSet& Messages, FText Message, const TScriptInterface<IBattleAnimation>& Animation, EAnimationPlacement AnimationPlacement = EAnimationPlacement::Before);
    
};