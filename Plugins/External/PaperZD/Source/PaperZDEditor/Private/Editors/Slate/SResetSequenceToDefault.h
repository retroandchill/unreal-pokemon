// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Editor/PropertyEditor/Public/PropertyHandle.h"
#include "Layout/Visibility.h"
#include "Input/Reply.h"

/**
 * 
 */
class SResetSequenceToDefault : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SResetSequenceToDefault)
	{}
	SLATE_END_ARGS()

	~SResetSequenceToDefault();

	void Construct(const FArguments &InArgs, const TSharedPtr< class IPropertyHandle>& InPropertyHandle);
	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	EVisibility GetDiffersFromDefaultAsVisibility() const;
	FReply OnResetClicked();

private:
	TSharedPtr< class IPropertyHandle > PropertyHandle;
	bool bValueDiffersFromDefault;
};
