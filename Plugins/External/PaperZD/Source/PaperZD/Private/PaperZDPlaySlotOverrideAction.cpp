// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDPlaySlotOverrideAction.h"
#include "PaperZDAnimInstance.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "UObject/Stack.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDPlaySlotOverrideAction)
#endif

UPaperZDPlaySlotOverrideAction* UPaperZDPlaySlotOverrideAction::PlayAnimationOverrideWithCallbacks(UPaperZDAnimInstance* AnimInstance, const UPaperZDAnimSequence* AnimSequence, UObject* WorldContextObject, FName SlotName, float PlayRate, float StartingPosition)
{
	UPaperZDPlaySlotOverrideAction* Node = NewObject<UPaperZDPlaySlotOverrideAction>();
	Node->SlotName = SlotName;
	Node->AnimSequencePtr = AnimSequence;
	Node->AnimInstancePtr = AnimInstance;
	Node->PlayRate = PlayRate;
	Node->StartingPosition = StartingPosition;
	Node->RegisterWithGameInstance(WorldContextObject);

	return Node;
}

void UPaperZDPlaySlotOverrideAction::Activate()
{
	if (AnimSequencePtr.IsValid() && AnimInstancePtr.IsValid())
	{
		FZDOnAnimationOverrideEndSignature OnEndDelegate;
		OnEndDelegate.BindUObject(this, &UPaperZDPlaySlotOverrideAction::OnAnimationOverrideEnd);

		//Play the override, can still fail due to invalid slot
		const bool bValid = AnimInstancePtr->PlayAnimationOverride(AnimSequencePtr.Get(), SlotName, PlayRate, StartingPosition, OnEndDelegate);
		if (bValid)
		{
			FFrame::KismetExecutionMessage(TEXT("Playing animation override..."), ELogVerbosity::Log);
			return;
		}
	}

	//Failed, mark and destroy
	FFrame::KismetExecutionMessage(TEXT("Could not play animation override"), ELogVerbosity::Log);
	SetReadyToDestroy();
}

void UPaperZDPlaySlotOverrideAction::OnAnimationOverrideEnd(bool bCompleted)
{
	if (bCompleted)
	{
		OnCompleted.Broadcast();
	}
	else
	{
		OnCancelled.Broadcast();
	}

	//Remove from game instance
	SetReadyToDestroy();
}
