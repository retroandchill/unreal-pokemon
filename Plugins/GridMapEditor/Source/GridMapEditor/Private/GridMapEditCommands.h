#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FGridMapEditCommands : public TCommands<FGridMapEditCommands>
{
public:
	FGridMapEditCommands();

	/**
	 * Grid Map Edit Commands
	 */

	/** Commands for the tools toolbar. */
	TSharedPtr<FUICommandInfo> SetPaintTiles;
	TSharedPtr<FUICommandInfo> SetSelectTiles;
	TSharedPtr<FUICommandInfo> SetTileSettings;

	/*
	TSharedPtr< FUICommandInfo > SetReapplySettings;	
	TSharedPtr< FUICommandInfo > SetLassoSelect;
	TSharedPtr< FUICommandInfo > SetPaintBucket;
	*/

	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;

private:
	void MakeUICommand(TSharedPtr<FUICommandInfo>& OutCommand, const TCHAR* CommandName, const FText&  FriendlyName, const FText& InDescription, const EUserInterfaceActionType CommandType, const FInputChord& InDefaultChord = FInputChord());
};