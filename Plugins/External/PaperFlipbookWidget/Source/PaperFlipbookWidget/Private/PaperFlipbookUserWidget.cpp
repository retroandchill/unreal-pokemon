//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "PaperFlipbookUserWidget.h"
#include "Slate/SlateBrushAsset.h"
#include "SPaperFlipbookWidget.h"

#define LOCTEXT_NAMESPACE "Paper2D"

/////////////////////////////////////////////////////
// UPaperFlipbookUserWidget

UPaperFlipbookUserWidget::UPaperFlipbookUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, SourceFlipbook(nullptr)
	, bAutoPlay(true)
	, PlayRate(1.0f)
	, bLooping(true)
	, ColorAndOpacity(FLinearColor::White)
	, bOverrideContentSize(false)
	, DesiredSize(0.f, 0.f)
{

}

void UPaperFlipbookUserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyFlipbookWG.Reset();
}

TSharedRef<SWidget> UPaperFlipbookUserWidget::RebuildWidget()
{
	MyFlipbookWG = SNew(SPaperFlipbookWidget);
	return MyFlipbookWG.ToSharedRef();
}

void UPaperFlipbookUserWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);

	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->OverrideBrushSize(DesiredSize, bOverrideContentSize);
		MyFlipbookWG->SetBrushMirroring(Mirroring);
		MyFlipbookWG->SetBrushTiling(Tiling);
		MyFlipbookWG->SetColorAndOpacity(ColorAndOpacityBinding);
		MyFlipbookWG->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
		MyFlipbookWG->SetOnFinishedPlaying(BIND_UOBJECT_DELEGATE(FSimpleDelegate, OnFlipbookFinishedPlaying));
		MyFlipbookWG->SetFlipbook(SourceFlipbook);
		MyFlipbookWG->SetPlayRate(PlayRate);
		MyFlipbookWG->SetLooping(bLooping);

		if (bAutoPlay)
		{
			MyFlipbookWG->Play();
		}
		else
		{
			MyFlipbookWG->Stop();
		}
	}
}

void UPaperFlipbookUserWidget::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UPaperFlipbookUserWidget::SetOpacity(float InOpacity)
{
	ColorAndOpacity.A = InOpacity;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UPaperFlipbookUserWidget::OverrideBrushSize(FVector2D InDesiredSize)
{
	if (!bOverrideContentSize || DesiredSize != InDesiredSize)
	{
		bOverrideContentSize = true;
		if (MyFlipbookWG.IsValid())
		{
			MyFlipbookWG->OverrideBrushSize(DesiredSize, bOverrideContentSize);
		}
	}
}

void UPaperFlipbookUserWidget::SetBrushTintColor(FSlateColor TintColor)
{
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetBrushTintColor(TintColor);
	}
}

void UPaperFlipbookUserWidget::SetBrushTiling(TEnumAsByte<enum ESlateBrushTileType::Type> InTiling)
{
	if (Tiling != InTiling)
	{
		Tiling = InTiling;
		if (MyFlipbookWG.IsValid())
		{
			MyFlipbookWG->SetBrushTiling(InTiling);
		}
	}
}

void UPaperFlipbookUserWidget::SetBrushMirroring(TEnumAsByte<enum ESlateBrushMirrorType::Type> InMirroring)
{
	if (Mirroring != InMirroring)
	{
		Mirroring = InMirroring;
		if (MyFlipbookWG.IsValid())
		{
			MyFlipbookWG->SetBrushMirroring(InMirroring);
		}
	}
}

FReply UPaperFlipbookUserWidget::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (OnMouseButtonDownEvent.IsBound())
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

bool UPaperFlipbookUserWidget::SetFlipbook(class UPaperFlipbook* NewFlipbook)
{
	if (NewFlipbook != SourceFlipbook)
	{
		SourceFlipbook = NewFlipbook;
		if (MyFlipbookWG.IsValid())
		{
			MyFlipbookWG->SetFlipbook(NewFlipbook);
		}
		return true;
	}

	return false;
}

UPaperFlipbook* UPaperFlipbookUserWidget::GetFlipbook()
{
	return SourceFlipbook;
}

void UPaperFlipbookUserWidget::Play()
{
	bAutoPlay = true;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->Play();
	}
}

void UPaperFlipbookUserWidget::PlayFromStart()
{
	bAutoPlay = true;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->PlayFromStart();
	}
}

void UPaperFlipbookUserWidget::Reverse()
{
	bAutoPlay = true;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->Reverse();
	}
}

void UPaperFlipbookUserWidget::ReverseFromEnd()
{
	bAutoPlay = true;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->ReverseFromEnd();
	}
}

void UPaperFlipbookUserWidget::Stop()
{
	bAutoPlay = false;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->Stop();
	}
}

bool UPaperFlipbookUserWidget::IsPlaying() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->IsPlaying();
	}
	return false;
}

bool UPaperFlipbookUserWidget::IsReversing() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->IsReversing();
	}
	return false;
}

void UPaperFlipbookUserWidget::SetPlaybackPositionInFrames(int32 NewFramePosition)
{
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetPlaybackPositionInFrames(NewFramePosition, false);
	}
}

int32 UPaperFlipbookUserWidget::GetPlaybackPositionInFrames() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetPlaybackPositionInFrames();
	}
	return 0;
}

void UPaperFlipbookUserWidget::SetPlaybackPosition(float NewPosition)
{
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetPlaybackPosition(NewPosition, false);
	}
}

float UPaperFlipbookUserWidget::GetPlaybackPosition() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetPlaybackPosition();
	}
	return 0.0f;
}

void UPaperFlipbookUserWidget::SetLooping(bool bNewLooping)
{
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetLooping(bNewLooping);
	}
}

bool UPaperFlipbookUserWidget::IsLooping() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->IsLooping();
	}
	return false;
}

void UPaperFlipbookUserWidget::SetPlayRate(float NewRate)
{
	PlayRate = NewRate;
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetPlayRate(NewRate);
	}
}

float UPaperFlipbookUserWidget::GetPlayRate() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetPlayRate();
	}
	return 0.0f;
}

void UPaperFlipbookUserWidget::SetNewTime(float NewTime)
{
	if (MyFlipbookWG.IsValid())
	{
		MyFlipbookWG->SetNewTime(NewTime);
	}
}

float UPaperFlipbookUserWidget::GetFlipbookLength() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetFlipbookLength();
	}
	return 0.0f;
}

int32 UPaperFlipbookUserWidget::GetFlipbookLengthInFrames() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetFlipbookLengthInFrames();
	}
	return 0;
}

float UPaperFlipbookUserWidget::GetFlipbookFramerate() const
{
	if (MyFlipbookWG.IsValid())
	{
		return MyFlipbookWG->GetFlipbookFramerate();
	}
	return 0.0f;
}

void UPaperFlipbookUserWidget::OnFlipbookFinishedPlaying()
{
	OnFinishedPlaying.Broadcast();
}

#if WITH_EDITOR

const FText UPaperFlipbookUserWidget::GetPaletteCategory()
{
	return LOCTEXT("Paper2D", "Paper2D");
}

#endif


/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
