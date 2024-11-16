// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "PaperZDAnimationSource_Flipbook.generated.h"

/**
 * An animation source to be used with Flipbook components.
 */
UCLASS()
class PAPERZD_API UPaperZDAnimationSource_Flipbook : public UPaperZDAnimationSource
{
	GENERATED_BODY()

public:
	//ctor
	UPaperZDAnimationSource_Flipbook();
	
	//~ Begin UPaperZDAnimationSource Interface
	virtual TSubclassOf<UPaperZDPlaybackHandle> GetPlaybackHandleClass() const override;
	virtual TSubclassOf<UPrimitiveComponent> GetRenderComponentClass() const override;
	//~ End UPaperZDAnimationSource Interface
};
