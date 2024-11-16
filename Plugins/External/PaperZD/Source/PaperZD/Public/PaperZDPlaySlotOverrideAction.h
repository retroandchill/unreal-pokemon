// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "PaperZDPlaySlotOverrideAction.generated.h"

class UPaperZDAnimSequence;
class UPaperZDAnimInstance;

//Pin outputs as delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPaperZDPlaySlotOverrideOutputSignature);

/**
 * Action handle for playing an animation override through an AnimInstance, while getting access to the completion events.
 */
UCLASS()
class PAPERZD_API UPaperZDPlaySlotOverrideAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/* Slot name that we want to play. */
	FName SlotName;

	/* PlayRate to use when updating the animation. */
	float PlayRate;

	/* Starting position of the playback. */
	float StartingPosition;

	/* AnimInstance target. */
	TWeakObjectPtr<UPaperZDAnimInstance> AnimInstancePtr;

	/* AnimSequence to use. */
	TWeakObjectPtr<const UPaperZDAnimSequence> AnimSequencePtr;

	/* Callback pin for when the override plays to its completion. */
	UPROPERTY(BlueprintAssignable)
	FPaperZDPlaySlotOverrideOutputSignature OnCompleted;

	/* Callback pin for when the override is canceled either by the user, or by being overridden by another slot in the same group. */
	UPROPERTY(BlueprintAssignable)
	FPaperZDPlaySlotOverrideOutputSignature OnCancelled;

public:	

	/**
	 * Play the given animation through the override slot.
	 * Triggers the OnCompleted/OnCancelled node when finished.
	 */
	UFUNCTION(BlueprintCallable, Category = "Playback", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UPaperZDPlaySlotOverrideAction* PlayAnimationOverrideWithCallbacks(UPaperZDAnimInstance* AnimInstance, const UPaperZDAnimSequence* AnimSequence, UObject* WorldContextObject, FName SlotName = "DefaultSlot", float PlayRate = 1.0f, float StartingPosition = 0.0f);

	/** Execute the actual operation */
	virtual void Activate() override;

private:
	/* Called when the animation overrides completes. */
	UFUNCTION()
	void OnAnimationOverrideEnd(bool bCompleted);
};
