// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Sequencer/IPaperZDSequencerSource.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(IPaperZDSequencerSource)
#endif

TSubclassOf<UPaperZDAnimInstance> IPaperZDSequencerSource::GetSequencerAnimInstanceClass() const
{
	return nullptr;
}

UPaperZDAnimInstance* IPaperZDSequencerSource::GetSequencerAnimInstance()
{
	return nullptr;
}
