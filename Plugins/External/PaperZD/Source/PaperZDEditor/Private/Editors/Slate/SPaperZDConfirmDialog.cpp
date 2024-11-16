// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "SPaperZDConfirmDialog.h"
#include "PaperZDEditor.h"
#include "Framework/Text/TextLayout.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Styling/AppStyle.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "PaperZD_ConfirmDialog"

void SPaperZDConfirmDialog::Construct(const FArguments& InArgs)
{
	bOkClicked = false;
	TitleText = InArgs._TitleText.EqualTo(FText::FromString(TEXT(""))) ? LOCTEXT("PaperZD_ConfirmDeleteTitle", "Confirm Action") : InArgs._TitleText;
	DetailText = InArgs._DetailText.EqualTo(FText::FromString(TEXT(""))) ? LOCTEXT("PaperZD_ConfirmDeleteDetail", "Are you sure you wan't to delete this? It cannot be undone!") : InArgs._DetailText;

	TAttribute<ETextJustify::Type> Justify(ETextJustify::Center);
	TSharedRef<STextBlock> TextBlock = SNew(STextBlock);
	TextBlock->SetText(DetailText);
	TextBlock->SetJustification(Justify);

	//Create the grid panel for the buttons
	TSharedPtr<SUniformGridPanel> ButtonGridPanel;

	if (InArgs._ShowCancelButton)
	{
		SAssignNew(ButtonGridPanel, SUniformGridPanel)
			.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
			.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
			.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
			+ SUniformGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SPaperZDConfirmDialog::OkClicked)
			.Text(LOCTEXT("CreateAnimBlueprintOk", "OK"))
			]
		+ SUniformGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SPaperZDConfirmDialog::CancelClicked)
			.Text(LOCTEXT("CreateAnimBlueprintCancel", "Cancel"))
			];
	}
	else
	{
		SAssignNew(ButtonGridPanel, SUniformGridPanel)
			.SlotPadding(FAppStyle::GetMargin("StandardDialog.SlotPadding"))
			.MinDesiredSlotWidth(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
			.MinDesiredSlotHeight(FAppStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
			+ SUniformGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.ContentPadding(FAppStyle::GetMargin("StandardDialog.ContentPadding"))
			.OnClicked(this, &SPaperZDConfirmDialog::OkClicked)
			.Text(LOCTEXT("CreateAnimBlueprintOk", "OK"))
			];
	}

	ChildSlot
	[
		SNew(SBorder)
		.Visibility(EVisibility::Visible)
		.BorderImage(FAppStyle::GetBrush("Menu.Background"))
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8)
			[
				TextBlock
			]

			+SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(8)
			[
				ButtonGridPanel->AsShared()
			]

		]
	];
}

bool SPaperZDConfirmDialog::Show()
{
	//Slate prepass first
	SlatePrepass(1.0f);

	//The desired size of this widget
	FVector2D Size = GetDesiredSize();

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(TitleText)
		.ClientSize(Size)
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.SizingRule(ESizingRule::FixedSize)
		[
			AsShared()
		];
	
	//Assign to the weak variable
	ContainingWindow = Window;
	GEditor->EditorAddModalWindow(Window);

	return bOkClicked;
}

FReply SPaperZDConfirmDialog::OkClicked()
{
	CloseDialog(true);
	return FReply::Handled();
}

FReply SPaperZDConfirmDialog::CancelClicked()
{
	CloseDialog();

	return FReply::Handled();
}

void SPaperZDConfirmDialog::CloseDialog(bool bWasPicked)
{
	bOkClicked = bWasPicked;
	if (ContainingWindow.IsValid())
	{
		ContainingWindow.Pin()->RequestDestroyWindow();
	}
}

#undef LOCTEXT_NAMESPACE