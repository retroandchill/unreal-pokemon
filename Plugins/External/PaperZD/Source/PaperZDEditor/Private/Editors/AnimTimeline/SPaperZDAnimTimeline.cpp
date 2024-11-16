// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimTimeline.h"
#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "Editors/AnimTimeline/PaperZDAnimTimeSliderController.h"
#include "Editors/AnimTimeline/SPaperZDAnimTrackArea.h"
#include "Editors/AnimTimeline/SPaperZDAnimOutliner.h"
#include "Editors/AnimTimeline/SPaperZDAnimTimelineOverlay.h"
#include "Editors/AnimTimeline/SPaperZDAnimTimelineSplitterOverlay.h"
#include "Editors/Util/PaperZDEditorSettings.h"
#include "Editors/Util/PaperZDEditorCommands.h"
#include "Editors/PaperZDAnimationSourceViewportClient.h"

#include "EditorStyleSet.h"
#include "Fonts/FontMeasure.h"
#include "FrameNumberNumericInterface.h"
#include "ISequencerWidgetsModule.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "Fonts/FontMeasure.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SSpinBox.h"
#include "EditorWidgetsModule.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "MovieSceneFwd.h"

#define LOCTEXT_NAMESPACE "PaperZDEditor_AnimTimeline"

void SPaperZDAnimTimeline::Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InModel)
{
	TWeakPtr<FPaperZDAnimModel> WeakModel = InModel;
	Model = InModel;
	OnReceivedFocus = InArgs._OnReceivedFocus;

	//Create the attributes that will be used for the controller
	ViewRange = MakeAttributeLambda([WeakModel]() { return WeakModel.IsValid() ? WeakModel.Pin()->GetViewRange() : FAnimatedRange(0.0, 0.0); });

	TAttribute<EFrameNumberDisplayFormats> DisplayFormat = MakeAttributeLambda([]()
		{
			return GetDefault<UPaperZDEditorSettings>()->TimelineDisplayFormat;
		});

	TAttribute<FFrameRate> TickResolution = MakeAttributeLambda([WeakModel]()
		{
			int32 TickResolutionValue = WeakModel.IsValid() ? WeakModel.Pin()->GetTickResolution() : 0;
			return FFrameRate(TickResolutionValue, 1);
		});

	TAttribute<FFrameRate> DisplayRate = MakeAttributeLambda([WeakModel]()
		{
			int32 SequenceFrameRate = WeakModel.IsValid() ? FMath::RoundToInt(WeakModel.Pin()->GetFrameRate()) : 0;
			return FFrameRate(SequenceFrameRate, 1);
		});

	// Create the numeric type interface and pass it
	NumericTypeInterface = MakeShareable(new FFrameNumberInterface(DisplayFormat, 0, TickResolution, DisplayRate));
	FTimeSliderArgs TimeSliderArgs;
	{
 		TimeSliderArgs.ScrubPosition = MakeAttributeLambda([WeakModel]() { return WeakModel.IsValid() ? WeakModel.Pin()->GetScrubPosition() : FFrameTime(0); });
 		TimeSliderArgs.ViewRange = ViewRange;
 		TimeSliderArgs.PlaybackRange = MakeAttributeLambda([WeakModel]() { return WeakModel.IsValid() ? WeakModel.Pin()->GetPlaybackRange() : TRange<FFrameNumber>(0, 0); });
 		TimeSliderArgs.ClampRange = MakeAttributeLambda([WeakModel]() { return WeakModel.IsValid() ? WeakModel.Pin()->GetWorkingRange() : FAnimatedRange(0.0, 0.0); });
 		TimeSliderArgs.DisplayRate = DisplayRate;
 		TimeSliderArgs.TickResolution = TickResolution;
 		TimeSliderArgs.OnViewRangeChanged = FOnViewRangeChanged::CreateSP(&InModel.Get(), &FPaperZDAnimModel::HandleViewRangeChanged);
 		TimeSliderArgs.OnClampRangeChanged = FOnTimeRangeChanged::CreateSP(&InModel.Get(), &FPaperZDAnimModel::HandleWorkingRangeChanged);
 		TimeSliderArgs.IsPlaybackRangeLocked = true;
 		TimeSliderArgs.PlaybackStatus = EMovieScenePlayerStatus::Stopped;
 		TimeSliderArgs.NumericTypeInterface = NumericTypeInterface;
 		TimeSliderArgs.OnScrubPositionChanged = FOnScrubPositionChanged::CreateSP(this, &SPaperZDAnimTimeline::HandleScrubPositionChanged);
	}

	//Create the time slider and its controller
	TimeSliderController = MakeShareable(new FPaperZDAnimTimeSliderController(TimeSliderArgs, InModel, SharedThis(this)));
	TSharedRef<FPaperZDAnimTimeSliderController> TimeSliderControllerRef = TimeSliderController.ToSharedRef();

	// Create the top slider
	const bool bMirrorLabels = false;
	ISequencerWidgetsModule& SequencerWidgets = FModuleManager::Get().LoadModuleChecked<ISequencerWidgetsModule>("SequencerWidgets");
	TopTimeSlider = SequencerWidgets.CreateTimeSlider(TimeSliderControllerRef, bMirrorLabels);

	// Create bottom time range slider
	TSharedRef<ITimeSlider> BottomTimeRange = SequencerWidgets.CreateTimeRange(
		FTimeRangeArgs(
			EShowRange::ViewRange | EShowRange::WorkingRange | EShowRange::PlaybackRange,
			EShowRange::ViewRange | EShowRange::WorkingRange,
			TimeSliderControllerRef,
			EVisibility::Visible,
			NumericTypeInterface.ToSharedRef()
		),
		SequencerWidgets.CreateTimeRangeSlider(TimeSliderControllerRef)
	);

	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar)
		.Thickness(FVector2D(5.0f, 5.0f));

	//Make sure model is updated
	InModel->RefreshTracks();

 	TrackArea = SNew(SPaperZDAnimTrackArea, InModel, TimeSliderControllerRef);
	Outliner = SNew(SPaperZDAnimOutliner, InModel, TrackArea.ToSharedRef())
		.ExternalScrollbar(ScrollBar)
		.Clipping(EWidgetClipping::ClipToBounds)
		.FilterText_Lambda([this]() { return FilterText; });

	TrackArea->SetOutliner(Outliner);

	//Create transport control widget
	TSharedPtr<ITransportControl> TransportControl;
	{
		//We forward all the calls to the viewport client.
		TSharedRef<FPaperZDAnimationSourceViewportClient> ViewportClient = InModel->GetSceneViewportClient().Pin().ToSharedRef();
		FTransportControlArgs Args;
		Args.OnForwardPlay = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Forward);
		Args.OnBackwardPlay = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Backward);
		Args.OnForwardStep = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Forward_Step);
		Args.OnForwardEnd = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Forward_End);
		Args.OnBackwardStep = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Backward_Step);
		Args.OnBackwardEnd = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_Backward_End);
		Args.OnToggleLooping = FOnClicked::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::OnClick_ToggleLoop);
		Args.OnGetLooping = FOnGetLooping::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::IsLooping);
		Args.OnGetPlaybackMode = FOnGetPlaybackMode::CreateSP(ViewportClient, &FPaperZDAnimationSourceViewportClient::GetPlaybackMode);

		FEditorWidgetsModule& WidgetsModule = FModuleManager::LoadModuleChecked<FEditorWidgetsModule>("EditorWidgets");
		TransportControl = WidgetsModule.CreateTransportControl(Args);
	}

	ColumnFillCoefficients[0] = 0.2f;
	ColumnFillCoefficients[1] = 0.8f;

	TAttribute<float> FillCoefficient_0, FillCoefficient_1;
	{
		FillCoefficient_0.Bind(TAttribute<float>::FGetter::CreateSP(this, &SPaperZDAnimTimeline::GetColumnFillCoefficient, 0));
		FillCoefficient_1.Bind(TAttribute<float>::FGetter::CreateSP(this, &SPaperZDAnimTimeline::GetColumnFillCoefficient, 1));
	}

	const int32 Column0 = 0, Column1 = 1;
	const int32 Row0 = 0, Row1 = 1, Row2 = 2, Row3 = 3, Row4 = 4;

	const float CommonPadding = 3.f;
	const FMargin ResizeBarPadding(4.f, 0, 0, 0);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				[
					SNew(SGridPanel)
					.FillRow(1, 1.0f)
					.FillColumn(0, FillCoefficient_0)
					.FillColumn(1, FillCoefficient_1)

					// outliner search box
					+SGridPanel::Slot(Column0, Row0, SGridPanel::Layer(10))
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SAssignNew(SearchBox, SSearchBox)
							.HintText(LOCTEXT("FilterTracksHint", "Filter"))
							.OnTextChanged(this, &SPaperZDAnimTimeline::OnOutlinerSearchChanged)
						]
						+SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.AutoWidth()
						.Padding(2.0f, 0.0f, 2.0f, 0.0f)
						[
							SNew(SBox)
							.MinDesiredWidth(30.0f)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							[
								// Current Play Time 
								SNew(SSpinBox<double>)
								.Style(&FAppStyle::GetWidgetStyle<FSpinBoxStyle>("Sequencer.PlayTimeSpinBox"))
								.Value_Lambda([this]() -> double
								{
									return Model.Pin()->GetScrubPosition().Value;
								})
								.OnValueChanged(this, &SPaperZDAnimTimeline::SetPlayTime)
								.OnValueCommitted_Lambda([this](double InFrame, ETextCommit::Type)
								{
									SetPlayTime(InFrame);
								})
								.MinValue(TOptional<double>())
								.MaxValue(TOptional<double>())
								.TypeInterface(NumericTypeInterface)
								.Delta(this, &SPaperZDAnimTimeline::GetSpinboxDelta)
								.LinearDeltaSensitivity(25)
							]
						]
					]
					// main timeline area
					+SGridPanel::Slot(Column0, Row1, SGridPanel::Layer(10))
					.ColumnSpan(2)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						[
							SNew(SOverlay)
							+SOverlay::Slot()
							[
								SNew(SScrollBorder, Outliner.ToSharedRef())
								[
									SNew(SHorizontalBox)

									// outliner tree
									+SHorizontalBox::Slot()
									.FillWidth(FillCoefficient_0)
									[
										SNew(SBox)
										[
											Outliner.ToSharedRef()
										]
									]

									// track area
									+SHorizontalBox::Slot()
									.FillWidth(FillCoefficient_1)
									[
										SNew(SBox)
										.Padding(ResizeBarPadding)
										.Clipping(EWidgetClipping::ClipToBounds)
										[
											TrackArea.ToSharedRef()
										]
									]
								]
							]

							+SOverlay::Slot()
							.HAlign(HAlign_Right)
							[
								ScrollBar
							]
						]
					]

					// Transport controls
					+SGridPanel::Slot(Column0, Row3, SGridPanel::Layer(10))
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						TransportControl.ToSharedRef()
					]

					// Second column
					+SGridPanel::Slot(Column1, Row0)
					.Padding(ResizeBarPadding)
					.RowSpan(2)
					[
						SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
						[
							SNew(SSpacer)
						]
					]

					+SGridPanel::Slot(Column1, Row0, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew( SBorder )
						.BorderImage( FAppStyle::GetBrush("ToolPanel.GroupBorder") )
						.BorderBackgroundColor( FLinearColor(.50f, .50f, .50f, 1.0f ) )
						.Padding(0)
						.Clipping(EWidgetClipping::ClipToBounds)
						[
							TopTimeSlider.ToSharedRef()
						]
					]

					// Overlay that draws the tick lines
					+SGridPanel::Slot(Column1, Row1, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew(SPaperZDAnimTimelineOverlay, TimeSliderControllerRef)
						.Visibility( EVisibility::HitTestInvisible )
						.DisplayScrubPosition( false )
						.DisplayTickLines( true )
						.Clipping(EWidgetClipping::ClipToBounds)
						.PaintPlaybackRangeArgs(FPaintPlaybackRangeArgs(FAppStyle::GetBrush("Sequencer.Timeline.PlayRange_L"), FAppStyle::GetBrush("Sequencer.Timeline.PlayRange_R"), 6.f))
					]

					// Overlay that draws the scrub position
					+SGridPanel::Slot(Column1, Row1, SGridPanel::Layer(20))
					.Padding(ResizeBarPadding)
					[
						SNew(SPaperZDAnimTimelineOverlay, TimeSliderControllerRef)
						.Visibility( EVisibility::HitTestInvisible )
						.DisplayScrubPosition( true )
						.DisplayTickLines( false )
						.Clipping(EWidgetClipping::ClipToBounds)
					]

					// play range slider
					+SGridPanel::Slot(Column1, Row3, SGridPanel::Layer(10))
					.Padding(ResizeBarPadding)
					[
						SNew(SBorder)
						.BorderImage( FAppStyle::GetBrush("ToolPanel.GroupBorder") )
						.BorderBackgroundColor( FLinearColor(0.5f, 0.5f, 0.5f, 1.0f ) )
						.Clipping(EWidgetClipping::ClipToBounds)
						.Padding(0)
						[
							BottomTimeRange
						]
					]
				]
// 				+SOverlay::Slot()
// 				[
// 					// track area virtual splitter overlay
// 					SNew(SAnimTimelineSplitterOverlay)
// 					.Style(FAppStyle::Get(), "AnimTimeline.Outliner.Splitter")
// 					.Visibility(EVisibility::SelfHitTestInvisible)
// 
// 					+ SSplitter::Slot()
// 					.Value(FillCoefficient_0)
// 					.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &SAnimTimeline::OnColumnFillCoefficientChanged, 0))
// 					[
// 						SNew(SSpacer)
// 					]
// 
// 					+ SSplitter::Slot()
// 					.Value(FillCoefficient_1)
// 					.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &SAnimTimeline::OnColumnFillCoefficientChanged, 1))
// 					[
// 						SNew(SSpacer)
// 					]
// 				]
			]
		]
	];

}

FReply SPaperZDAnimTimeline::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();

		const bool bCloseAfterSelection = true;
		FMenuBuilder MenuBuilder(bCloseAfterSelection, Model.Pin()->GetCommandList());

		MenuBuilder.BeginSection("SnapOptions", LOCTEXT("SnapOptions", "Snapping"));
		{
			MenuBuilder.AddMenuEntry(FPaperZDEditorCommands::Get().SnapToFrames);
			MenuBuilder.AddMenuEntry(FPaperZDEditorCommands::Get().SnapToNotifies);
		}
		MenuBuilder.EndSection();

		MenuBuilder.BeginSection("TimelineOptions", LOCTEXT("TimelineOptions", "Timeline Options") );
		{
			MenuBuilder.AddSubMenu(
				LOCTEXT("TimeFormat", "Time Format"),
				LOCTEXT("TimeFormatTooltip", "Choose the format of times we display in the timeline"),
				FNewMenuDelegate::CreateLambda([](FMenuBuilder& InMenuBuilder)
				{
					InMenuBuilder.BeginSection("TimeFormat", LOCTEXT("TimeFormat", "Time Format") );
					{
						InMenuBuilder.AddMenuEntry(FPaperZDEditorCommands::Get().DisplaySeconds);
						InMenuBuilder.AddMenuEntry(FPaperZDEditorCommands::Get().DisplayFrames);
					}
					InMenuBuilder.EndSection();

					InMenuBuilder.BeginSection("TimelineAdditional", LOCTEXT("TimelineAdditional", "Additional Display") );
					{
						InMenuBuilder.AddMenuEntry(FPaperZDEditorCommands::Get().DisplayPercentage);
					}
					InMenuBuilder.EndSection();
				})
			);
		}
		MenuBuilder.EndSection();

		FSlateApplication::Get().PushMenu(SharedThis(this), WidgetPath, MenuBuilder.MakeWidget(), FSlateApplication::Get().GetCursorPos(), FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

// FFrameRate::ComputeGridSpacing doesnt deal well with prime numbers, so we have a custom impl here
static bool ComputeGridSpacing(const FFrameRate& InFrameRate, float PixelsPerSecond, double& OutMajorInterval, int32& OutMinorDivisions, float MinTickPx, float DesiredMajorTickPx)
{
	// First try built-in spacing
	bool bResult = InFrameRate.ComputeGridSpacing(PixelsPerSecond, OutMajorInterval, OutMinorDivisions, MinTickPx, DesiredMajorTickPx);
	if (!bResult || OutMajorInterval == 1.0)
	{
		if (PixelsPerSecond <= 0.f)
		{
			return false;
		}

		const int32 RoundedFPS = FMath::RoundToInt(InFrameRate.AsDecimal());

		if (RoundedFPS > 0)
		{
			// Showing frames
			TArray<int32, TInlineAllocator<10>> CommonBases;

			// Divide the rounded frame rate by 2s, 3s or 5s recursively
			{
				const int32 Denominators[] = { 2, 3, 5 };

				int32 LowestBase = RoundedFPS;
				for (;;)
				{
					CommonBases.Add(LowestBase);

					if (LowestBase % 2 == 0) { LowestBase = LowestBase / 2; }
					else if (LowestBase % 3 == 0) { LowestBase = LowestBase / 3; }
					else if (LowestBase % 5 == 0) { LowestBase = LowestBase / 5; }
					else
					{
						int32 LowestResult = LowestBase;
						for (int32 Denominator : Denominators)
						{
							int32 Result = LowestBase / Denominator;
							if (Result > 0 && Result < LowestResult)
							{
								LowestResult = Result;
							}
						}

						if (LowestResult < LowestBase)
						{
							LowestBase = LowestResult;
						}
						else
						{
							break;
						}
					}
				}
			}

			Algo::Reverse(CommonBases);

			const int32 Scale = FMath::CeilToInt(DesiredMajorTickPx / PixelsPerSecond * InFrameRate.AsDecimal());
			const int32 BaseIndex = FMath::Min(Algo::LowerBound(CommonBases, Scale), CommonBases.Num() - 1);
			const int32 Base = CommonBases[BaseIndex];

			int32 MajorIntervalFrames = FMath::CeilToInt(Scale / float(Base)) * Base;
			OutMajorInterval = MajorIntervalFrames * InFrameRate.AsInterval();

			// Find the lowest number of divisions we can show that's larger than the minimum tick size
			OutMinorDivisions = 0;
			for (int32 DivIndex = 0; DivIndex < BaseIndex; ++DivIndex)
			{
				if (Base % CommonBases[DivIndex] == 0)
				{
					int32 MinorDivisions = MajorIntervalFrames / CommonBases[DivIndex];
					if (OutMajorInterval / MinorDivisions * PixelsPerSecond >= MinTickPx)
					{
						OutMinorDivisions = MinorDivisions;
						break;
					}
				}
			}
		}
	}

	return OutMajorInterval != 0;
}

bool SPaperZDAnimTimeline::GetGridMetrics(float PhysicalWidth, double& OutMajorInterval, int32& OutMinorDivisions) const
{
	FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);
	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	FFrameRate DisplayRate(FMath::RoundToInt(Model.Pin()->GetFrameRate()), 1);
	double BiggestTime = ViewRange.Get().GetUpperBoundValue();
	FString TickString = NumericTypeInterface->ToString((BiggestTime * DisplayRate).FrameNumber.Value);
	FVector2D MaxTextSize = FontMeasureService->Measure(TickString, SmallLayoutFont);

	static float MajorTickMultiplier = 2.f;

	float MinTickPx = MaxTextSize.X + 5.f;
	float DesiredMajorTickPx = MaxTextSize.X * MajorTickMultiplier;

	if (PhysicalWidth > 0)
	{
		return ComputeGridSpacing(
			DisplayRate,
			PhysicalWidth / ViewRange.Get().Size<double>(),
			OutMajorInterval,
			OutMinorDivisions,
			MinTickPx,
			DesiredMajorTickPx);
	}

	return false;
}

TSharedRef<INumericTypeInterface<double>> SPaperZDAnimTimeline::GetNumericTypeInterface() const
{
	return NumericTypeInterface.ToSharedRef();
}

TSharedPtr<ITimeSliderController> SPaperZDAnimTimeline::GetTimeSliderController() const
{
	return TimeSliderController;
}

void SPaperZDAnimTimeline::OnOutlinerSearchChanged(const FText& Filter)
{
	FilterText = Filter;

	Outliner->RefreshFilter();
}

void SPaperZDAnimTimeline::OnColumnFillCoefficientChanged(float FillCoefficient, int32 ColumnIndex)
{
	ColumnFillCoefficients[ColumnIndex] = FillCoefficient;
}

void SPaperZDAnimTimeline::HandleScrubPositionChanged(FFrameTime NewScrubPosition, bool bIsScrubbing, bool bEvaluate)
{
	if (Model.IsValid())
	{
		TSharedPtr<FPaperZDAnimModel> ModelPtr = Model.Pin();
		if (ModelPtr->GetSceneViewportClient().IsValid())
		{
			TSharedPtr<FPaperZDAnimationSourceViewportClient> ViewportClient = ModelPtr->GetSceneViewportClient().Pin();
			if (ViewportClient->GetPlaybackMode() != EPlaybackMode::Stopped)
			{
				ViewportClient->SetPlaying(false);
			}
		}

		ModelPtr->SetScrubPosition(NewScrubPosition);
	}
}

double SPaperZDAnimTimeline::GetSpinboxDelta() const
{
	return FFrameRate(Model.Pin()->GetTickResolution(), 1).AsDecimal() * FFrameRate(FMath::RoundToInt(Model.Pin()->GetFrameRate()), 1).AsInterval();
}

void SPaperZDAnimTimeline::SetPlayTime(double InFrameTime)
{
	if (Model.IsValid())
	{
		TSharedPtr<FPaperZDAnimModel> ModelPtr = Model.Pin();
		if (ModelPtr->GetSceneViewportClient().IsValid())
		{
			TSharedPtr<FPaperZDAnimationSourceViewportClient> ViewportClient = ModelPtr->GetSceneViewportClient().Pin();
			if (ViewportClient->GetPlaybackMode() != EPlaybackMode::Stopped)
			{
				ViewportClient->SetPlaying(false);
				ViewportClient->SetPlaybackPosition(InFrameTime / (double)Model.Pin()->GetTickResolution());
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
