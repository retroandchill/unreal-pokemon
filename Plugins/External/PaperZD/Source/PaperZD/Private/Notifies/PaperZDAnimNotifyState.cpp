// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDAnimNotifyState.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotifyState)
#endif

//static defines
const float UPaperZDAnimNotifyState::MinimumStateDuration = (1.0f / 30.0f);

//State
UPaperZDAnimNotifyState::UPaperZDAnimNotifyState(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	Duration = MinimumStateDuration;
}

void UPaperZDAnimNotifyState::TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
{
	//Super takes care of setting world context, etc
	Super::TickNotify(DeltaTime, Playtime, LastPlaybackTime, AnimRenderComponent, bPersistentActiveState, OwningInstance);

	//We'll work on the notify by pieces, shaving off delta time to account for edge cases like looping and edge markers being on the start/end frames
	float RemainingDeltaTime = DeltaTime;

	//Different case scenarios when going backwards
	if (DeltaTime > 0.0f)
	{
		//First figure out if this notify was active last frame, if so we need to tick it first
		const float EndTime = Time + Duration;
		const bool bWasActive = bPersistentActiveState;
		if (bWasActive)
		{
			const float TickTime = FMath::Min(RemainingDeltaTime, EndTime - LastPlaybackTime);
			OnNotifyTick(TickTime, OwningInstance);

			//Shave off the tick time we just used
			RemainingDeltaTime -= TickTime;
		}

		//Check if we used all our time in this step
		if (RemainingDeltaTime <= 0.0f)
		{
			return;
		}

		//We still have time left, if we were active, then that means we became inactive
		if (bWasActive)
		{
			OnNotifyEnd(OwningInstance);
			bPersistentActiveState = false;
		}

		//At this point, we know for sure we at least we are out of the notify play-range.
		//Check if with the remainder time we got inside it.
		const bool bCurrentlyActive = Playtime > Time && Playtime < EndTime;
		if (bCurrentlyActive)
		{
			//The previous step handled notifies that were already active, so if we got to this point
			//this meant that the notify got activated in this frame
			OnNotifyBegin(OwningInstance);

			//Then just tick any remainder time
			const float TickTime = FMath::Min(RemainingDeltaTime, Duration);
			OnNotifyTick(TickTime, OwningInstance);

			//Shave off the time, any remaining time will be ignored
			RemainingDeltaTime -= TickTime;

			//Ask the persistent storage to remember that we're active for next iteration
			bPersistentActiveState = true;

			//If we got to this point, then this means the notify looped more than once, which shouldn't happen ever
			if (RemainingDeltaTime > 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("Notify state '%s' finished tick with remainder time, this normally means the sequence looped more than once in the same frame. Begin and end events will only be called once"), *GetName());
			}
		}
	}
	else
	{
		//Same logic as above, with the relevant markers flipped
		//First figure out if this notify was active last frame, if so we need to tick it first
		const float EndTime = Time + Duration;
		const bool bWasActive = bPersistentActiveState;
		if (bWasActive)
		{
			const float TickTime = FMath::Max(RemainingDeltaTime, Time - LastPlaybackTime);
			OnNotifyTick(TickTime, OwningInstance);

			//Shave off the tick time we just used
			RemainingDeltaTime -= TickTime;
		}

		//Check if we used all our time in this step
		if (RemainingDeltaTime >= 0.0f)
		{
			return;
		}

		//We still have time left, if we were active, then that means we became inactive
		if (bWasActive)
		{
			OnNotifyEnd(OwningInstance);
			bPersistentActiveState = false;
		}

		//At this point, we know for sure we at least we are out of the notify play-range.
		//Check if with the remainder time we got inside it.
		const bool bCurrentlyActive = Playtime > Time && Playtime < EndTime;
		if (bCurrentlyActive)
		{
			//The previous step handled notifies that were already active, so if we got to this point
			//this meant that the notify got activated in this frame
			OnNotifyBegin(OwningInstance);

			//Then just tick any remainder time
			const float TickTime = FMath::Max(RemainingDeltaTime, -Duration);
			OnNotifyTick(TickTime, OwningInstance);

			//Shave off the time, any remaining time will be ignored
			RemainingDeltaTime -= TickTime;

			//Ask the persistent storage to remember that we're active for next iteration
			bPersistentActiveState = true;

			//If we got to this point, then this means the notify looped more than once, which shouldn't happen ever
			if (RemainingDeltaTime < 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("Notify state '%s' finished tick with remainder time, this normally means the sequence looped more than once in the same frame. Begin and end events will only be called once"), *GetName());
			}
		}
	}
}

void UPaperZDAnimNotifyState::OnNotifyBegin_Implementation(UPaperZDAnimInstance *OwningInstance /* = nullptr*/) const
{
	//Empty Implementation
}

void UPaperZDAnimNotifyState::OnNotifyTick_Implementation(float DeltaTime, UPaperZDAnimInstance *OwningInstance /* = nullptr*/) const
{
	//Empty Implementation
}

void UPaperZDAnimNotifyState::OnNotifyEnd_Implementation(UPaperZDAnimInstance *OwningInstance /* = nullptr*/) const
{
	//Empty Implementation
}

void UPaperZDAnimNotifyState::OnNotifyAborted(UPaperZDAnimInstance* OwningInstance) const
{
	OnNotifyEnd(OwningInstance);
}
