//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Styling/CoreStyle.h"
#include "Widgets/SLeafWidget.h"

class FPaintArgs;
class FSlateWindowElementList;
class UPaperFlipbook;
class UPaperSprite;

/**
 * Implements a widget that displays a paper flipbook.
 */
class PAPERFLIPBOOKWIDGET_API SPaperFlipbookWidget
	: public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperFlipbookWidget)
		: _ColorAndOpacity(FLinearColor::White)
		, _OnMouseButtonDown()
	{
	}


	/** Color and opacity */
	SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

		/** Invoked when the mouse is pressed in the widget. */
		SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)

		/** Called when the button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnFinishedPlaying)

		SLATE_END_ARGS()

		/** Constructor */
		SPaperFlipbookWidget();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

public:

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** See OnMouseButtonDown event */
	void SetOnMouseButtonDown(FPointerEventHandler EventHandler);

	/** Set OnFinishedPlaying event */
	void SetOnFinishedPlaying(FSimpleDelegate InOnFinishedPlaying);

	void OverrideBrushSize(FVector2D InDesiredSize, bool bOverride);

	/**  */
	void SetBrushTintColor(FSlateColor TintColor);

	/**  */
	void SetBrushTiling(TEnumAsByte<enum ESlateBrushTileType::Type> InTiling);

	/**  */
	void SetBrushMirroring(TEnumAsByte<enum ESlateBrushMirrorType::Type> InMirroring);

	FSlateBrush* GetBrush();

public:

	// SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

protected:
	// Begin SWidget overrides.
	virtual FVector2D ComputeDesiredSize(float) const override;
	// End SWidget overrides.

protected:

	FSlateBrush DefaultBrush;

	uint8 bUseSpriteSize : 1;

	/** Color and opacity scale for this image */
	TAttribute<FSlateColor> ColorAndOpacity;

	/** Invoked when the mouse is pressed in the image */
	FPointerEventHandler OnMouseButtonDownHandler;

	/** The delegate to execute when a non-looping flipbook finishes playing */
	FSimpleDelegate OnFinishedPlaying;

	/** Flipbook currently being played */
	UPaperFlipbook* SourceFlipbook;

	/** Current play rate of the flipbook */
	float PlayRate;

	/** Whether the flipbook should loop when it reaches the end, or stop */
	uint8 bLooping : 1;

	/** If playback should move the current position backwards instead of forwards */
	uint8 bReversePlayback : 1;

	/** Are we currently playing (moving Position) */
	uint8 bPlaying : 1;

	/** Current position in the timeline */
	float AccumulatedTime;

	/** Last frame index calculated */
	int32 CachedFrameIndex;

public:
	/** Change the flipbook used by this instance (will reset the play time to 0 if it is a new flipbook). */
	virtual bool SetFlipbook(class UPaperFlipbook* NewFlipbook);

	/** Gets the flipbook used by this instance. */
	virtual UPaperFlipbook* GetFlipbook();

	/** Start playback of flipbook */
	void Play();

	/** Start playback of flipbook from the start */
	void PlayFromStart();

	/** Start playback of flipbook in reverse */
	void Reverse();

	/** Start playback of flipbook in reverse from the end */
	void ReverseFromEnd();

	/** Stop playback of flipbook */
	void Stop();

	/** Get whether this flipbook is playing or not. */
	bool IsPlaying() const;

	/** Get whether we are reversing or not */
	bool IsReversing() const;

	/** Jump to a position in the flipbook (expressed in frames). If bFireEvents is true, event functions will fire, otherwise they will not. */
	void SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents);

	/** Get the current playback position (in frames) of the flipbook */
	int32 GetPlaybackPositionInFrames() const;

	/** Jump to a position in the flipbook (expressed in seconds). If bFireEvents is true, event functions will fire, otherwise they will not. */
	void SetPlaybackPosition(float NewPosition, bool bFireEvents);

	/** Get the current playback position (in seconds) of the flipbook */
	float GetPlaybackPosition() const;

	/** true means we should loop, false means we should not. */
	void SetLooping(bool bNewLooping);

	/** Get whether we are looping or not */
	bool IsLooping() const;

	/** Sets the new play rate for this flipbook */
	void SetPlayRate(float NewRate);

	/** Get the current play rate for this flipbook */
	float GetPlayRate() const;

	/** Set the new playback position time to use */
	void SetNewTime(float NewTime);

	/** Get length of the flipbook (in seconds) */
	float GetFlipbookLength() const;

	/** Get length of the flipbook (in frames) */
	int32 GetFlipbookLengthInFrames() const;

	/** Get the nominal framerate that the flipbook will be played back at (ignoring PlayRate), in frames per second */
	float GetFlipbookFramerate() const;

protected:

	void CalculateCurrentFrame();
	UPaperSprite* GetSpriteAtCachedIndex() const;

	void TickFlipbook(float DeltaTime);
};
