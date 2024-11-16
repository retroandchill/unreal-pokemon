// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDAnimNotify.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotify)
#endif

UPaperZDAnimNotify::UPaperZDAnimNotify(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDAnimNotify::TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
{
	//Super takes care of setting world context object
	Super::TickNotify(DeltaTime, Playtime, LastPlaybackTime, AnimRenderComponent, bPersistentActiveState, OwningInstance);

	//Normal Notifies tick only once, look for the time
	if (DeltaTime > 0.0f)
	{
		const bool bLooped = Playtime < LastPlaybackTime;
		if (bLooped && (Playtime >= Time || LastPlaybackTime <= Time))
		{
			OnReceiveNotify(OwningInstance);
		}
		else if (Playtime > Time && LastPlaybackTime <= Time)
		{
			OnReceiveNotify(OwningInstance);
		}
	}
	else
	{
		const bool bLooped = Playtime > LastPlaybackTime;
		if (bLooped && (Playtime <= Time || LastPlaybackTime >= Time))
		{
			OnReceiveNotify(OwningInstance);
		}
		else if (Playtime < Time && LastPlaybackTime >= Time)
		{
			OnReceiveNotify(OwningInstance);
		}
	}
}

void UPaperZDAnimNotify::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance /* = nullptr*/) const
{
	//Empty implementation
}
