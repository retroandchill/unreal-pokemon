// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Notifies/PaperZDAnimNotify_Base.h"
#include "PaperZDAnimNotifyState.generated.h"

UCLASS(Blueprintable, Abstract)
class PAPERZD_API UPaperZDAnimNotifyState : public UPaperZDAnimNotify_Base
{
	GENERATED_UCLASS_BODY()

public:
	/** The minimum possible duration that a notify state can have */
	static const float MinimumStateDuration;

	/* Duration of this state notify. */
	UPROPERTY(BlueprintReadOnly, Category = "PaperZD")
	float Duration;

	//Called each Tick to process the notify and trigger it when necessary
	virtual void TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance = nullptr) const override;

	UFUNCTION(BlueprintNativeEvent, Category="PaperZD")
	void OnNotifyBegin(UPaperZDAnimInstance* OwningInstance) const;

	UFUNCTION(BlueprintNativeEvent, Category = "PaperZD")
	void OnNotifyTick(float DeltaTime, UPaperZDAnimInstance* OwningInstance) const;

	UFUNCTION(BlueprintNativeEvent, Category = "PaperZD")
	void OnNotifyEnd(UPaperZDAnimInstance* OwningInstance) const;

protected:
	/* Called when the notify state gets aborted before it ends naturally. */
	virtual void OnNotifyAborted(UPaperZDAnimInstance* OwningInstance) const override;
};
