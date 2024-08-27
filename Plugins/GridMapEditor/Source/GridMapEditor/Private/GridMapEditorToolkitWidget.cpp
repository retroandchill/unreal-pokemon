#include "GridMapEditorToolkitWidget.h"
#include "Editor.h"
#include "EditorModeManager.h"
#include "EditorStyleSet.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "GridMapEditCommands.h"
#include "GridMapEditorMode.h"
#include "GridMapStyleSet.h"
#include "SlateOptMacros.h"
#include "TileSet.h"
#include "Widgets/GridMapEditorSettingsWidget.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Notifications/SErrorText.h"
#include "Widgets/STileSetPalette.h"
#include "WorkflowOrientedApp/SContentReference.h"

#define LOCTEXT_NAMESPACE "GridMapEditor"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGridMapEditorToolkitWidget::Construct(const FArguments& InArgs)
{
	GridMapEditorMode = (FGridMapEditorMode*)GLevelEditorModeTools().GetActiveMode(FGridMapEditorMode::EM_GridMapEditorModeId);

	const FText BlankText = FText::GetEmpty();

	ChildSlot[
		SNew(SVerticalBox)
		// Error text goes in the first vertical slot
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0,0,0,5)
		[
			SAssignNew(ErrorText, SErrorText)
		]
		// Main content comes next
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			// Container for left side "tabs" and right content
			SNew(SVerticalBox)
			.IsEnabled(this, &SGridMapEditorToolkitWidget::IsGridMapEditorEnabled)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				// left side "tabs"
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(1.f, 5.f, 0.f, 5.f)
				[
					BuildToolBar()
				]
				// right side content
				+ SHorizontalBox::Slot()
				.Padding(0.f, 2.f, 2.f, 0.f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
					.Padding(FGridMapStyleSet::StandardPadding)
					[
						SNew(SVerticalBox)

						// Active Tool Title
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.Padding(FGridMapStyleSet::StandardLeftPadding)
							.HAlign(HAlign_Left)
							[
								SNew(STextBlock)
								.Text(this, &SGridMapEditorToolkitWidget::GetActiveToolName)
								.TextStyle(FEditorStyle::Get(), "FoliageEditMode.ActiveToolName.Text")
							]
						]
						
						// Paint Options
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							BuildPaintOptions()
						]

						// Settings Options
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SGridMapEditorSettingsWidget, &GridMapEditorMode->UISettings, GridMapEditorMode)
						]

					]
				]
			]

			// Foliage Palette
			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			.VAlign(VAlign_Fill)
			.Padding(0.f, 5.f, 0.f, 0.f)
			[
				SAssignNew(TileSetPalette, STileSetPalette)
				.GridMapEditorMode(GridMapEditorMode)
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SGridMapEditorToolkitWidget::BuildToolBar()
{
	FVerticalToolBarBuilder Toolbar(GridMapEditorMode->UICommandList, FMultiBoxCustomization::None);
	Toolbar.SetLabelVisibility(EVisibility::Collapsed);
	Toolbar.SetStyle(&FEditorStyle::Get(), "FoliageEditToolbar");
	{
		Toolbar.AddToolBarButton(FGridMapEditCommands::Get().SetPaintTiles);
		Toolbar.AddToolBarButton(FGridMapEditCommands::Get().SetSelectTiles);
		Toolbar.AddToolBarButton(FGridMapEditCommands::Get().SetTileSettings);
		
		/*
		Toolbar.AddToolBarButton(FFoliageEditCommands::Get().SetReapplySettings);
		Toolbar.AddToolBarButton(FFoliageEditCommands::Get().SetLassoSelect);
		Toolbar.AddToolBarButton(FFoliageEditCommands::Get().SetPaintBucket);
		*/
	}

	return
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.HAlign(HAlign_Center)
				.Padding(0)
				.BorderImage(FEditorStyle::GetBrush("NoBorder"))
				.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
				[
					Toolbar.MakeWidget()
				]
			]
		];
}

TSharedRef<SWidget> SGridMapEditorToolkitWidget::BuildPaintOptions()
{
	return 
		SNew(SVerticalBox)
		.Visibility(this, &SGridMapEditorToolkitWidget::GetVisibility_PaintOptions)
		// Brush Options
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FGridMapStyleSet::StandardPadding)
		[
			SNew(SHeader)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("OptionHeader", "Tile Options"))
				.Font(FGridMapStyleSet::StandardFont)
			]
		]

		//
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			.ToolTipText(LOCTEXT("GridMapActiveTileset_ToolTip", "Currently Active TileSet"))

			+ SHorizontalBox::Slot()
			.Padding(FGridMapStyleSet::StandardLeftPadding)
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("GridMapActiveTileset", "Tiles"))
				.Font(FGridMapStyleSet::StandardFont)
			]

			+ SHorizontalBox::Slot()
			.Padding(FGridMapStyleSet::StandardRightPadding)
			.FillWidth(2.0f)
			.MaxWidth(140.f)
			.VAlign(VAlign_Center)
			[
				SNew(SContentReference)
				.WidthOverride(140.f)
				.AssetReference(this, &SGridMapEditorToolkitWidget::GetCurrentTileSet)
				.OnSetReference(this, &SGridMapEditorToolkitWidget::OnChangeTileSet)
				.AllowedClass(UGridMapTileSet::StaticClass())
				.AllowSelectingNewAsset(true)
				.AllowClearingReference(false)
			]								
		]

		// Tilemap Target Height
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			.ToolTipText(LOCTEXT("GridMapPaintHeight_ToolTip", "Paint height of the gridmap"))

			+ SHorizontalBox::Slot()
			.Padding(FGridMapStyleSet::StandardLeftPadding)
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("GridMapPaintHeight", "Height"))
				.Font(FGridMapStyleSet::StandardFont)
			]
#if false // temp
			+ SHorizontalBox::Slot()
			.Padding(FGridMapStyleSet::StandardRightPadding)
			.FillWidth(2.0f)
			.MaxWidth(100.f)
			.VAlign(VAlign_Center)
			[
				SNew(SNumericEntryBox<float>)
				.Font(FGridMapStyleSet::StandardFont)
				.AllowSpin(true)
				.MinValue(-65530.0f)
				.MaxValue(65530.0f)
				.MaxSliderValue(1000.f)
				.MinDesiredValueWidth(50.0f)
				.SliderExponent(3.0f)
				.Value(this, &SGridMapEditorToolkitWidget::GetPaintHeight)
				.OnValueChanged(this, &SGridMapEditorToolkitWidget::SetPaintHeight)
			]
#endif
		]

		// Hide Actors
		+ SVerticalBox::Slot()
		.Padding(FGridMapStyleSet::StandardPadding)
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(FGridMapStyleSet::StandardPadding)
			[
				SNew(SWrapBox)
				.UseAllottedWidth(true)
				.InnerSlotPadding({6, 5})

				+ SWrapBox::Slot()
				[
					SNew(SBox)
					.MinDesiredWidth(150)
					[
						SNew(SCheckBox)
						.OnCheckStateChanged(this, &SGridMapEditorToolkitWidget::OnCheckStateChanged_HideOwnedActors)
						.IsChecked(this, &SGridMapEditorToolkitWidget::GetCheckState_HideOwnedActors)
						.ToolTipText(LOCTEXT("GridMapHideActors_ToolTip", "Whether to hide the actors in the outliner"))
						[
							SNew(STextBlock)
							.Text(LOCTEXT("GridMapHideActors", "Hide in outliner"))
							.Font(FGridMapStyleSet::StandardFont)
						]
					]
				]
			]
		];
}

bool SGridMapEditorToolkitWidget::IsGridMapEditorEnabled() const
{
	ErrorText->SetError(GetGridMapEditorErrorText());
	return GridMapEditorMode->IsEditingEnabled();
}

bool SGridMapEditorToolkitWidget::IsPaintTool() const
{
	return GridMapEditorMode->UISettings.GetPaintToolSelected();
}

bool SGridMapEditorToolkitWidget::IsSelectTool() const
{
	return GridMapEditorMode->UISettings.GetSelectToolSelected();
}

bool SGridMapEditorToolkitWidget::IsSettingsTool() const
{
	return GridMapEditorMode->UISettings.GetSettingsToolSelected();
}

void SGridMapEditorToolkitWidget::SetPaintOrigin(FVector NewPaintOrigin)
{
	GridMapEditorMode->UISettings.SetPaintOrigin(NewPaintOrigin);
}

TOptional<FVector> SGridMapEditorToolkitWidget::GetPaintOrigin() const
{
	return GridMapEditorMode->UISettings.GetPaintOrigin();
}

void SGridMapEditorToolkitWidget::OnChangeTileSet(UObject* NewAsset)
{
	if (UGridMapTileSet* NewTileSet = Cast<UGridMapTileSet>(NewAsset))
	{
		GridMapEditorMode->UISettings.SetCurrentTileSet(NewTileSet);
	}
}

UObject* SGridMapEditorToolkitWidget::GetCurrentTileSet() const
{
	return GridMapEditorMode->UISettings.GetCurrentTileSet().Get();
}


void SGridMapEditorToolkitWidget::OnCheckStateChanged_HideOwnedActors(ECheckBoxState InState)
{
	GridMapEditorMode->UISettings.SetHideOwnedActors(InState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGridMapEditorToolkitWidget::GetCheckState_HideOwnedActors() const
{
	return GridMapEditorMode->UISettings.GetHideOwnedActors() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}


FText SGridMapEditorToolkitWidget::GetActiveToolName() const
{
	const FGridMapEditCommands& Commands = FGridMapEditCommands::Get();
	if (IsPaintTool())
	{
		return Commands.SetPaintTiles->GetLabel();
	}
	else if (IsSelectTool())
	{
		return Commands.SetSelectTiles->GetLabel();
	}
	else if (IsSettingsTool())
	{
		return Commands.SetTileSettings->GetLabel();
	}
	/*
	else if (IsReapplySettingsTool())
	{
		OutText = LOCTEXT("FoliageToolName_Reapply", "Reapply");
	}
	
	else if (IsLassoSelectTool())
	{
		OutText = LOCTEXT("FoliageToolName_LassoSelect", "Lasso Select");
	}
	else if (IsPaintFillTool())
	{
		OutText = LOCTEXT("FoliageToolName_Fill", "Fill");
	}
	*/

	return FText::GetEmpty();
}

FText SGridMapEditorToolkitWidget::GetGridMapEditorErrorText() const
{
	EGridMapEditingState EditState = GridMapEditorMode->GetEditingState();

	switch (EditState)
	{
	case EGridMapEditingState::SIEWorld: return LOCTEXT("IsSimulatingError_edit", "Can't edit grid map while simulating!");
	case EGridMapEditingState::PIEWorld: return LOCTEXT("IsPIEError_edit", "Can't edit grid map in PIE!");
	case EGridMapEditingState::Enabled: return FText::GetEmpty();
	default: checkNoEntry();
	}
	
	return FText::GetEmpty();
}

EVisibility SGridMapEditorToolkitWidget::GetVisibility_PaintOptions() const
{
	if (IsPaintTool())
	{
		return EVisibility::Visible;
	}

	return EVisibility::Collapsed;
}

EVisibility SGridMapEditorToolkitWidget::GetVisibility_SelectOptions() const
{
	if (IsSelectTool())
	{
		return EVisibility::Visible;
	}

	return EVisibility::Collapsed;
}

EVisibility SGridMapEditorToolkitWidget::GetVisibility_SettingsOptions() const
{
	if (IsSettingsTool())
	{
		return EVisibility::Visible;
	}

	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE