// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/AppStyle.h"
#include "Framework/Commands/Commands.h"

/**
 * Commands for the PaperZD Editor module
 */
class FPaperZDEditorCommands : public TCommands<FPaperZDEditorCommands>
{
public:
	//ctor
	FPaperZDEditorCommands()
		: TCommands<FPaperZDEditorCommands>(TEXT("PaperZDEditorCommands"), NSLOCTEXT("PaperZDContexts", "PaperZDEditorCommands", "PaperZD Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{}

	virtual void RegisterCommands() override;

	//AnimSequence Related
	TSharedPtr<FUICommandInfo> CreateAnimSequence;
	TSharedPtr<FUICommandInfo> SaveSelectedAsset;

	//Timeline related
	TSharedPtr<FUICommandInfo> AddNotifyTrack;
	TSharedPtr<FUICommandInfo> InsertNotifyTrack;
	TSharedPtr<FUICommandInfo> RemoveNotifyTrack;
	TSharedPtr<FUICommandInfo> SnapToFrames;
	TSharedPtr<FUICommandInfo> SnapToNotifies;
	TSharedPtr<FUICommandInfo> DisplaySeconds;
	TSharedPtr<FUICommandInfo> DisplayFrames;
	TSharedPtr<FUICommandInfo> DisplayPercentage;
};
