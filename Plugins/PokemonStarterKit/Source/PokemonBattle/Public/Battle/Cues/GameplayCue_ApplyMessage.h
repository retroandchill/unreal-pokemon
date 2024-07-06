// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCue_ApplyMessage.generated.h"

struct FRunningMessageSet;

/**
 * Apply a battle message to the current running set when the effect is applied
 */
UCLASS(Abstract, Blueprintable)
class POKEMONBATTLE_API UGameplayCue_ApplyMessage : public UGameplayCueNotify_Static {
    GENERATED_BODY()

protected:
    /**
     * Perform the message append for whatever trigger is desired
     * @param MyTarget The target of the gameplay cue
     * @param Parameters The parameters of the cue call
     * @return Did the application of the cue succeed?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Battle|Display")
    bool PerformMessageAppend(AActor *MyTarget, const FGameplayCueParameters &Parameters) const;

    /**
     * Append a new message to the list of selected messages
     * @param TargetActor The actor that is being targed by the effect
     * @param Parameters The parameters passed to the gameplay cue
     * @param Messages The message to append
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Display")
    void AppendMessage(AActor* TargetActor, const FGameplayCueParameters& Parameters, const FRunningMessageSet& Messages) const;
};
