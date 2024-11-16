// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/NumericTypeInterface.h"
#include "Templates/SharedPointer.h"
#include "ITimeSlider.h"
#include "ITransportControl.h"

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 3
#include "AnimatedRange.h"
#endif

class FPaperZDAnimModel;
class FPaperZDAnimTimeSliderController;
class SPaperZDAnimOutliner;
class SPaperZDAnimTrackArea;
class SSearchBox;

/**
 * Implements the PaperZD version of the AnimTimeline used in the persona editor
 */
class SPaperZDAnimTimeline : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimTimeline)
	{}
	SLATE_EVENT(FSimpleDelegate, OnReceivedFocus)
	SLATE_END_ARGS()

public:
	/* Constructs the widget. */
	void Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InAnimModel);

	/** SWidget interface */
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	/** Compute a major grid interval and number of minor divisions to display */
	bool GetGridMetrics(float PhysicalWidth, double& OutMajorInterval, int32& OutMinorDivisions) const;

	/** Get the time slider controller */
	TSharedPtr<ITimeSliderController> GetTimeSliderController() const;

private:
	/**
	 * @return The fill percentage of the animation outliner
	 */
	float GetColumnFillCoefficient(int32 ColumnIndex) const
	{
		return ColumnFillCoefficients[ColumnIndex];
	}

	/** Get numeric Type interface for converting between frame numbers and display formats. */
	TSharedRef<INumericTypeInterface<double>> GetNumericTypeInterface() const;

	/** Called when the outliner search terms change */
	void OnOutlinerSearchChanged(const FText& Filter);

	/** Called when a column fill percentage is changed by a splitter slot. */
	void OnColumnFillCoefficientChanged(float FillCoefficient, int32 ColumnIndex);

	/* Handles the change of the position scrub and updates the viewport to reflect it. */
	void HandleScrubPositionChanged(FFrameTime NewScrubPosition, bool bIsScrubbing, bool bEvaluate);

	double GetSpinboxDelta() const;

	void SetPlayTime(double InFrameTime);

private:
	/* Pointer to the model that represents the animation asset being rendered. */
	TWeakPtr<FPaperZDAnimModel> Model;

	/** Outliner widget */
	TSharedPtr<SPaperZDAnimOutliner> Outliner;

	/** Track area widget */
	TSharedPtr<SPaperZDAnimTrackArea> TrackArea;

	/** Called when any widget contained within the anim timeline has received focus */
	FSimpleDelegate OnReceivedFocus;

	/** The time slider controller */
	TSharedPtr<FPaperZDAnimTimeSliderController> TimeSliderController;

	/** The top time slider widget */
	TSharedPtr<ITimeSlider> TopTimeSlider;

	/** The search box for filtering tracks. */
	TSharedPtr<SSearchBox> SearchBox;

	/** The fill coefficients of each column in the grid. */
	float ColumnFillCoefficients[2];
// 
// 	/** Called when the user has begun dragging the selection selection range */
// 	FSimpleDelegate OnSelectionRangeBeginDrag;
// 
// 	/** Called when the user has finished dragging the selection selection range */
// 	FSimpleDelegate OnSelectionRangeEndDrag;
// 
// 	/** Called when any widget contained within the anim timeline has received focus */
// 	FSimpleDelegate OnReceivedFocus;

	/** Numeric Type interface for converting between frame numbers and display formats. */
	TSharedPtr<INumericTypeInterface<double>> NumericTypeInterface;

	/** The view range */
	TAttribute<FAnimatedRange> ViewRange;

	/** Filter text used to search the tree */
	FText FilterText;
};
