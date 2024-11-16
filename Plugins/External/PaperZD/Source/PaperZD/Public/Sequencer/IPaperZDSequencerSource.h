// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "IPaperZDSequencerSource.generated.h"

class UPaperZDAnimInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPaperZDSequencerSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * Represents an object that can work as a Sequencer animation source.
 */
class PAPERZD_API IPaperZDSequencerSource
{
	GENERATED_BODY()

public:
	/* Obtain the AnimInstance class that is used for driving the animations. */
	virtual TSubclassOf<UPaperZDAnimInstance> GetSequencerAnimInstanceClass() const;

	/* Get the AnimInstance for use in sequencer, should create it if it doesn't exist. */
	virtual UPaperZDAnimInstance* GetSequencerAnimInstance();
};
