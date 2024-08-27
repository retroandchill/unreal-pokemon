// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GridMapEditorModeToolkit.h"
#include "GridMapEditorMode.h"
#include "GridMapEditorToolkitWidget.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "GridMapEditor"

FGridMapEditorModeToolkit::FGridMapEditorModeToolkit()
{
}

void FGridMapEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	ToolkitWidget = SNew(SGridMapEditorToolkitWidget);
	FModeToolkit::Init(InitToolkitHost);
}

FName FGridMapEditorModeToolkit::GetToolkitFName() const
{
	return FName("GridMapEditorEdMode");
}

FText FGridMapEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("GridMapEditorToolkitName", "Grid Map Editor Toolkit");
}

class FEdMode* FGridMapEditorModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FGridMapEditorMode::EM_GridMapEditorModeId);
}

TSharedPtr<class SWidget> FGridMapEditorModeToolkit::GetInlineContent() const
{
	return ToolkitWidget;
}

#undef LOCTEXT_NAMESPACE
