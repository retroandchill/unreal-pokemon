// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.
#include "PaperZDEditorSettings.h"
#include "FrameNumberDisplayFormat.h"

UPaperZDEditorSettings::UPaperZDEditorSettings() : Super()
{
	SequencePlacementPolicy = EAnimSequencePlacementPolicy::SubFolder;
	SequencePlacementFolderName = TEXT("AnimSequences");
	SequenceDuplicationFolderName = TEXT("AnimSequences_Copy");
	ResultNodeTitleColor = FLinearColor(1.0f, 0.65f, 0.4f, 1.0f);
	bAutomaticallyPurgeUnregisteredNotifyFunctions = true;

	//Timeline
	TimelineScrubSnapValue = 1000;
	TimelineDisplayFormat = EFrameNumberDisplayFormats::Frames;
	TimelineEnabledSnaps = { TEXT("Frames") };
}

bool UPaperZDEditorSettings::CanEditChange(const FProperty* InProperty) const
{
	const FName PropertyName = InProperty->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UPaperZDEditorSettings, SequencePlacementFolderName))
	{
		return SequencePlacementPolicy == EAnimSequencePlacementPolicy::SubFolder;
	}

	return Super::CanEditChange(InProperty);
}
