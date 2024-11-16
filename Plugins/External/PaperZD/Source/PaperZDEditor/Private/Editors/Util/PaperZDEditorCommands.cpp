// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Util/PaperZDEditorCommands.h"


#define LOCTEXT_NAMESPACE "PaperZDAnimBPEditorCommands"

/** UI_COMMAND takes long for the compile to optimize */
UE_DISABLE_OPTIMIZATION
void FPaperZDEditorCommands::RegisterCommands()
{
	//AnimSequence commands
	UI_COMMAND(CreateAnimSequence, "New Animation Sequence", "Creates a new AnimSequence and registers it to this AnimationSource for further use.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SaveSelectedAsset, "Save", "Save the selected asset(s).", EUserInterfaceActionType::Button, FInputChord(EKeys::S, EModifierKey::Control));

	//Timeline commands
	UI_COMMAND(AddNotifyTrack, "Add Notify Track", "Add a new notify track", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(InsertNotifyTrack, "Insert Notify Track", "Insert a new notify track above here", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RemoveNotifyTrack, "Remove Notify Track", "Remove this notify track", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SnapToFrames, "Frames", "Snap to frame boundaries", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SnapToNotifies, "Notifies", "Snap to notifies and notify states", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(DisplaySeconds, "Seconds", "Display the time in seconds", EUserInterfaceActionType::RadioButton, FInputChord());
	UI_COMMAND(DisplayFrames, "Frames", "Display the time in frames", EUserInterfaceActionType::RadioButton, FInputChord());
	UI_COMMAND(DisplayPercentage, "Percentage", "Display the percentage along with the time scrubber", EUserInterfaceActionType::ToggleButton, FInputChord());
}

UE_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
