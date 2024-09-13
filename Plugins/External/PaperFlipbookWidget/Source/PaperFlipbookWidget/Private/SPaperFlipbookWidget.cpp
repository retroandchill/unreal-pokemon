//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "SPaperFlipbookWidget.h"
#include "Rendering/DrawElements.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PaperSpriteBlueprintLibrary.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "PaperFlipbookWidgetStats.h"


DECLARE_CYCLE_STAT(TEXT("Tick Flipbook Widget"), STAT_TickFlipbookWidget, STATGROUP_FlipbookWidget);
DECLARE_CYCLE_STAT(TEXT("On Paint Flipbook Widget"), STAT_OnPaint, STATGROUP_FlipbookWidget);


SPaperFlipbookWidget::SPaperFlipbookWidget()
{
	SetCanTick(true);
	bCanSupportFocus = false;

	DefaultBrush.DrawAs = ESlateBrushDrawType::Image;
	DefaultBrush.Tiling = ESlateBrushTileType::NoTile;
	DefaultBrush.Mirroring = ESlateBrushMirrorType::NoMirror;

	CachedFrameIndex = INDEX_NONE;
	AccumulatedTime = 0.0f;
	PlayRate = 1.0f;

	bLooping = true;
	bReversePlayback = false;
	bPlaying = true;
	bUseSpriteSize = true;
}

void SPaperFlipbookWidget::Construct(const FArguments& InArgs)
{
	ColorAndOpacity = InArgs._ColorAndOpacity;
	OnMouseButtonDownHandler = InArgs._OnMouseButtonDown;
}

int32 SPaperFlipbookWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	SCOPE_CYCLE_COUNTER(STAT_OnPaint);

	if (DefaultBrush.DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
		const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * DefaultBrush.GetTint(InWidgetStyle));


		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), &DefaultBrush, DrawEffects, FinalColorAndOpacity);
	}
	return LayerId;
}

FReply SPaperFlipbookWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (OnMouseButtonDownHandler.IsBound())
	{
		// If a handler is assigned, call it.
		return OnMouseButtonDownHandler.Execute(MyGeometry, MouseEvent);
	}
	else
	{
		// otherwise the event is unhandled.
		return FReply::Unhandled();
	}

}

void SPaperFlipbookWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SLeafWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	{
		SCOPE_CYCLE_COUNTER(STAT_TickFlipbookWidget);

		// Advance time
		TickFlipbook(InDeltaTime);

		// Update the frame and push it to the renderer if necessary
		CalculateCurrentFrame();
	}
}

FVector2D SPaperFlipbookWidget::ComputeDesiredSize(float) const
{
	return DefaultBrush.ImageSize;
}

void SPaperFlipbookWidget::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	if (!ColorAndOpacity.IdenticalTo(InColorAndOpacity))
	{
		ColorAndOpacity = InColorAndOpacity;
		Invalidate(EInvalidateWidget::PaintAndVolatility);
	}
}

void SPaperFlipbookWidget::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	if (!ColorAndOpacity.IdenticalTo(InColorAndOpacity))
	{
		ColorAndOpacity = InColorAndOpacity;
		Invalidate(EInvalidateWidget::PaintAndVolatility);
	}
}

void SPaperFlipbookWidget::SetOnMouseButtonDown(FPointerEventHandler EventHandler)
{
	OnMouseButtonDownHandler = EventHandler;
}

void SPaperFlipbookWidget::SetOnFinishedPlaying(FSimpleDelegate InOnFinishedPlaying)
{
	OnFinishedPlaying = InOnFinishedPlaying;
}

void SPaperFlipbookWidget::OverrideBrushSize(FVector2D InDesiredSize, bool bOverride)
{
	bUseSpriteSize = !bOverride;
	if (bOverride)
	{
		DefaultBrush.ImageSize = InDesiredSize;
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

void SPaperFlipbookWidget::SetBrushTintColor(FSlateColor TintColor)
{
	if (DefaultBrush.TintColor != TintColor)
	{
		DefaultBrush.TintColor = TintColor;
		Invalidate(EInvalidateWidget::PaintAndVolatility);
	}
}

void SPaperFlipbookWidget::SetBrushTiling(TEnumAsByte<enum ESlateBrushTileType::Type> InTiling)
{
	DefaultBrush.Tiling = InTiling;
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

void SPaperFlipbookWidget::SetBrushMirroring(TEnumAsByte<enum ESlateBrushMirrorType::Type> InMirroring)
{
	DefaultBrush.Mirroring = InMirroring;
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

FSlateBrush* SPaperFlipbookWidget::GetBrush()
{
	return &DefaultBrush;
}

UPaperSprite* SPaperFlipbookWidget::GetSpriteAtCachedIndex() const
{
	UPaperSprite* SpriteToSend = nullptr;
	if ((SourceFlipbook != nullptr) && SourceFlipbook->IsValidKeyFrameIndex(CachedFrameIndex))
	{
		SpriteToSend = SourceFlipbook->GetKeyFrameChecked(CachedFrameIndex).Sprite;
	}
	return SpriteToSend;
}

void SPaperFlipbookWidget::CalculateCurrentFrame()
{
	const int32 LastCachedFrame = CachedFrameIndex;
	CachedFrameIndex = (SourceFlipbook != nullptr) ? SourceFlipbook->GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;

	if (CachedFrameIndex != LastCachedFrame)
	{
		if (UPaperSprite* Sprite = GetSpriteAtCachedIndex())
		{
			const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
			const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();
			DefaultBrush.SetResourceObject(Sprite);

			if (bUseSpriteSize)
			{
				DefaultBrush.ImageSize = SpriteSize;
			}
		}
		else
		{
			DefaultBrush.SetResourceObject(nullptr);
		}

		// Indicate we need to send new dynamic data.
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}


void SPaperFlipbookWidget::TickFlipbook(float DeltaTime)
{
	bool bIsFinished = false;

	if (bPlaying)
	{
		const float TimelineLength = GetFlipbookLength();
		const float EffectiveDeltaTime = DeltaTime * (bReversePlayback ? (-PlayRate) : (PlayRate));

		float NewPosition = AccumulatedTime + EffectiveDeltaTime;

		if (EffectiveDeltaTime > 0.0f)
		{
			if (NewPosition > TimelineLength)
			{
				if (bLooping)
				{
					// If looping, play to end, jump to start, and set target to somewhere near the beginning.
					SetPlaybackPosition(TimelineLength, true);
					SetPlaybackPosition(0.0f, false);

					if (TimelineLength > 0.0f)
					{
						while (NewPosition > TimelineLength)
						{
							NewPosition -= TimelineLength;
						}
					}
					else
					{
						NewPosition = 0.0f;
					}
				}
				else
				{
					// If not looping, snap to end and stop playing.
					NewPosition = TimelineLength;
					Stop();
					bIsFinished = true;
				}
			}
		}
		else
		{
			if (NewPosition < 0.0f)
			{
				if (bLooping)
				{
					// If looping, play to start, jump to end, and set target to somewhere near the end.
					SetPlaybackPosition(0.0f, true);
					SetPlaybackPosition(TimelineLength, false);

					if (TimelineLength > 0.0f)
					{
						while (NewPosition < 0.0f)
						{
							NewPosition += TimelineLength;
						}
					}
					else
					{
						NewPosition = 0.0f;
					}
				}
				else
				{
					// If not looping, snap to start and stop playing.
					NewPosition = 0.0f;
					Stop();
					bIsFinished = true;
				}
			}
		}

		SetPlaybackPosition(NewPosition, true);
	}

	// Notify user that the flipbook finished playing
	if (bIsFinished)
	{
		OnFinishedPlaying.ExecuteIfBound();
	}
}

bool SPaperFlipbookWidget::SetFlipbook(class UPaperFlipbook* NewFlipbook)
{
	if (NewFlipbook != SourceFlipbook)
	{
		// Don't allow changing the sprite if we are "static".
		/*AActor* ComponentOwner = GetOwner();
		if ((ComponentOwner == nullptr) || AreDynamicDataChangesAllowed())*/
		{
			SourceFlipbook = NewFlipbook;

			// We need to also reset the frame and time also
			AccumulatedTime = 0.0f;
			CalculateCurrentFrame();

			Invalidate(EInvalidateWidget::LayoutAndVolatility);
			return true;
		}
	}

	return false;
}

UPaperFlipbook* SPaperFlipbookWidget::GetFlipbook()
{
	return SourceFlipbook;
}


void SPaperFlipbookWidget::Play()
{
	bReversePlayback = false;
	bPlaying = true;
}

void SPaperFlipbookWidget::PlayFromStart()
{
	SetPlaybackPosition(0.0f, /*bFireEvents=*/ false);
	Play();
}

void SPaperFlipbookWidget::Reverse()
{
	bReversePlayback = true;
	bPlaying = true;
}

void SPaperFlipbookWidget::ReverseFromEnd()
{
	SetPlaybackPosition(GetFlipbookLength(), /*bFireEvents=*/ false);
	Reverse();
}

void SPaperFlipbookWidget::Stop()
{
	bPlaying = false;
}

bool SPaperFlipbookWidget::IsPlaying() const
{
	return bPlaying;
}

bool SPaperFlipbookWidget::IsReversing() const
{
	return bPlaying && bReversePlayback;
}

void SPaperFlipbookWidget::SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents)
{
	const float Framerate = GetFlipbookFramerate();
	const float NewTime = (Framerate > 0.0f) ? (NewFramePosition / Framerate) : 0.0f;
	SetPlaybackPosition(NewTime, bFireEvents);
}

int32 SPaperFlipbookWidget::GetPlaybackPositionInFrames() const
{
	const float Framerate = GetFlipbookFramerate();
	const int32 NumFrames = GetFlipbookLengthInFrames();
	if (NumFrames > 0)
	{
		return FMath::Clamp<int32>(FMath::TruncToInt(AccumulatedTime * Framerate), 0, NumFrames - 1);
	}
	else
	{
		return 0;
	}
}

void SPaperFlipbookWidget::SetPlaybackPosition(float NewPosition, bool bFireEvents)
{
	float OldPosition = AccumulatedTime;
	AccumulatedTime = NewPosition;

	// If we should be firing events for this track...
	if (bFireEvents)
	{
		float MinTime;
		float MaxTime;
		if (!bReversePlayback)
		{
			// If playing sequence forwards.
			MinTime = OldPosition;
			MaxTime = AccumulatedTime;

			// Slight hack here.. if playing forwards and reaching the end of the sequence, force it over a little to ensure we fire events actually on the end of the sequence.
			if (MaxTime == GetFlipbookLength())
			{
				MaxTime += (float)KINDA_SMALL_NUMBER;
			}
		}
		else
		{
			// If playing sequence backwards.
			MinTime = AccumulatedTime;
			MaxTime = OldPosition;

			// Same small hack as above for backwards case.
			if (MinTime == 0.0f)
			{
				MinTime -= (float)KINDA_SMALL_NUMBER;
			}
		}

#if 0
		// See which events fall into traversed region.
		for (int32 i = 0; i < Events.Num(); i++)
		{
			float EventTime = Events[i].Time;

			// Need to be slightly careful here and make behavior for firing events symmetric when playing forwards of backwards.
			bool bFireThisEvent = false;
			if (!bReversePlayback)
			{
				if ((EventTime >= MinTime) && (EventTime < MaxTime))
				{
					bFireThisEvent = true;
				}
			}
			else
			{
				if ((EventTime > MinTime) && (EventTime <= MaxTime))
				{
					bFireThisEvent = true;
				}
			}

			if (bFireThisEvent)
			{
				Events[i].EventFunc.ExecuteIfBound();
			}
		}
#endif
	}

#if 0
	// Execute the delegate to say that all properties are updated
	TimelinePostUpdateFunc.ExecuteIfBound();
#endif

	if (OldPosition != AccumulatedTime)
	{
		CalculateCurrentFrame();
	}
}

float SPaperFlipbookWidget::GetPlaybackPosition() const
{
	return AccumulatedTime;
}

void SPaperFlipbookWidget::SetLooping(bool bNewLooping)
{
	bLooping = bNewLooping;
}

bool SPaperFlipbookWidget::IsLooping() const
{
	return bLooping;
}

void SPaperFlipbookWidget::SetPlayRate(float NewRate)
{
	PlayRate = NewRate;
}

float SPaperFlipbookWidget::GetPlayRate() const
{
	return PlayRate;
}

void SPaperFlipbookWidget::SetNewTime(float NewTime)
{
	// Ensure value is sensible
	//@TODO: PAPER2D: NewTime = FMath::Clamp<float>(NewTime, 0.0f, Length);

	SetPlaybackPosition(NewTime, /*bFireEvents=*/ false);
}

float SPaperFlipbookWidget::GetFlipbookLength() const
{
	return (SourceFlipbook != nullptr) ? SourceFlipbook->GetTotalDuration() : 0.0f;
}

int32 SPaperFlipbookWidget::GetFlipbookLengthInFrames() const
{
	return (SourceFlipbook != nullptr) ? SourceFlipbook->GetNumFrames() : 0;
}

float SPaperFlipbookWidget::GetFlipbookFramerate() const
{
	return (SourceFlipbook != nullptr) ? SourceFlipbook->GetFramesPerSecond() : 15.0f;
}

