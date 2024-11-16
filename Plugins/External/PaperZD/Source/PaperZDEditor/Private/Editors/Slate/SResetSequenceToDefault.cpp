// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "SResetSequenceToDefault.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Styling/AppStyle.h"

SResetSequenceToDefault::~SResetSequenceToDefault()
{
	if (PropertyHandle.IsValid())
	{
		PropertyHandle->ClearResetToDefaultCustomized();
	}
}

void SResetSequenceToDefault::Construct(const FArguments &InArgs, const TSharedPtr<class IPropertyHandle>& InPropertyHandle)
{
	PropertyHandle = InPropertyHandle;
	bValueDiffersFromDefault = false;

	if (InPropertyHandle.IsValid())
	{
		InPropertyHandle->MarkResetToDefaultCustomized();
	}

	ChildSlot
		[
			SNew(SButton)
			.IsFocusable(false)
			//.ToolTipText(this, &SResetToDefaultPropertyEditor::GetResetToolTip)
			.ButtonStyle( FAppStyle::Get(), "NoBorder" )
			.ContentPadding(0) 
			.Visibility( this, &SResetSequenceToDefault::GetDiffersFromDefaultAsVisibility )
			.OnClicked( this, &SResetSequenceToDefault::OnResetClicked )
			.Content()
			[
				SNew(SImage)
				.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
			]
		];
}

void SResetSequenceToDefault::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (PropertyHandle.IsValid())
	{
		bValueDiffersFromDefault = PropertyHandle->CanResetToDefault();
	}
}

FReply SResetSequenceToDefault::OnResetClicked()
{
	if (PropertyHandle.IsValid())
	{
		PropertyHandle->ResetToDefault();
	}
	
	return FReply::Handled();
}

EVisibility SResetSequenceToDefault::GetDiffersFromDefaultAsVisibility() const
{
	return bValueDiffersFromDefault ? EVisibility::Visible : EVisibility::Hidden;
}