// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"


class SPaperZDConfirmDialog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperZDConfirmDialog) :
		_TitleText(FText::FromString(TEXT(""))),
		_DetailText(FText::FromString(TEXT(""))),
		_ShowCancelButton(true)
	{
	}
	SLATE_ARGUMENT(FText, TitleText)
	SLATE_ARGUMENT(FText, DetailText)
	SLATE_ARGUMENT(bool, ShowCancelButton)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	//Show this widget as a modal window, will not return until the modal window is closed
	bool Show();

	//Close the dialog, using the selected option
	void CloseDialog(bool bWasPicked = false);

	FReply OkClicked();
	FReply CancelClicked();

private:
	bool bOkClicked;

	//The window that holds this widget
	TWeakPtr<SWindow> ContainingWindow;

	//The title to show on the window
	FText TitleText;

	//The detail to show on the body
	FText DetailText;
};
