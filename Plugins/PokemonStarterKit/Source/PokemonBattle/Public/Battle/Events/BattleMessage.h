// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "BattleMessage.generated.h"

class UGameplayAbility;
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
struct POKEMONBATTLE_API FBattleMessage
#if CPP
    : public FGCObject {
#else
{
#endif
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
    explicit FBattleMessage(FText &&Message, const TScriptInterface<IBattleAnimation> &Animation,
                            EAnimationPlacement AnimationPlacement);

    void AddReferencedObjects(FReferenceCollector &Collector) override;
    FString GetReferencerName() const override;
};