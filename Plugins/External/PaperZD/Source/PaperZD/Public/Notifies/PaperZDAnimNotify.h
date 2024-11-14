// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Notifies/PaperZDAnimNotify_Base.h"
#include "PaperZDAnimNotify.generated.h"

UCLASS(abstract, Blueprintable)
class PAPERZD_API UPaperZDAnimNotify : public UPaperZDAnimNotify_Base
{
	GENERATED_UCLASS_BODY()

public:
	//Called each Tick to process the notify and trigger it when necessary
	virtual void TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance = nullptr) const override;

	UFUNCTION(BlueprintNativeEvent, Category = "PaperZD")
	void OnReceiveNotify(UPaperZDAnimInstance* OwningInstance = nullptr) const;	
};
