// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Notifies/PaperZDAnimNotify.h"
#include "PaperZDAnimNotifyCustom.generated.h"

/**
 * A Custom notify gets compiled and can only be used inside the AnimBP that registered it.
 */
UCLASS()
class PAPERZD_API UPaperZDAnimNotifyCustom : public UPaperZDAnimNotify
{
	GENERATED_UCLASS_BODY()
			
public:
	//Override the native notify implementation
	void OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance = nullptr) const override;

#if WITH_EDITOR
	/* Custom notifies cannot be placed directly. */
	virtual bool CanBePlaced(UPaperZDAnimSequence* Animation) const override { return false; }
#endif
};
