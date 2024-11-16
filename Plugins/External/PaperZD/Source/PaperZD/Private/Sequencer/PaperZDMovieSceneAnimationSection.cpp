// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Sequencer/PaperZDMovieSceneAnimationSection.h"
#include "MovieScene.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "MovieSceneTimeHelpers.h"
#include "AnimSequences/PaperZDAnimSequence.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDMovieSceneAnimationSection)
#endif

#define LOCTEXT_NAMESPACE "PaperZDMovieSceneAnimationSection"

FPaperZDMovieSceneAnimationParams::FPaperZDMovieSceneAnimationParams()
{
	Animation = nullptr;
	StartOffset = 0.f;
	EndOffset = 0.f;
	PlayRate = 1.f;
	bReverse = false;
	Weight.SetDefault(1.f);
	SlotName = TEXT("DefaultSlot");
}

float FPaperZDMovieSceneAnimationParams::GetDuration() const
{
	return FMath::IsNearlyZero(PlayRate) || !Animation->IsValidLowLevel() ? 0.f : Animation->GetTotalDuration() / PlayRate;
}

float FPaperZDMovieSceneAnimationParams::GetSequenceLength() const
{
	return Animation->IsValidLowLevel() ? Animation->GetTotalDuration() : 0.f;
}

//////////////////////////////////////////////////////////////////////////
//// Section
//////////////////////////////////////////////////////////////////////////
UPaperZDMovieSceneAnimationSection::UPaperZDMovieSceneAnimationSection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Section template relies on always restoring state for objects when they are no longer animating. This is how it releases animation control.
	EvalOptions.CompletionMode = EMovieSceneCompletionMode::RestoreState;
	
#if WITH_EDITOR
	//Store Playrate
	PreviousPlayRate = Params.PlayRate;
	BlendType = EMovieSceneBlendType::Absolute;

	//Weight setup
	static FMovieSceneChannelMetaData MetaData("Weight", LOCTEXT("WeightChannelName", "Weight"));
	MetaData.bCanCollapseToTrack = false;
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(Params.Weight, MetaData, TMovieSceneExternalValue<float>());
#else
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(Params.Weight);
#endif
}

TOptional<TRange<FFrameNumber> > UPaperZDMovieSceneAnimationSection::GetAutoSizeRange() const
{
	FFrameRate FrameRate = GetTypedOuter<UMovieScene>()->GetTickResolution();

	FFrameTime AnimationLength = Params.GetSequenceLength() * FrameRate;

	return TRange<FFrameNumber>(GetInclusiveStartFrame(), GetInclusiveStartFrame() + AnimationLength.FrameNumber);
}

float UPaperZDMovieSceneAnimationSection::GetStartOffsetAtTrimTime(FQualifiedFrameTime TrimTime, const FPaperZDMovieSceneAnimationParams& InParams, FFrameNumber StartFrame)
{
	float AnimPlayRate = FMath::IsNearlyZero(InParams.PlayRate) ? 1.0f : InParams.PlayRate;
	float AnimPosition = (TrimTime.Time - StartFrame) / TrimTime.Rate * AnimPlayRate;
	float SeqLength = InParams.GetSequenceLength() - (InParams.StartOffset + InParams.EndOffset);

	float NewOffset = FMath::Fmod(AnimPosition, SeqLength);
	NewOffset += InParams.StartOffset;

	return NewOffset;
}

void UPaperZDMovieSceneAnimationSection::TrimSection(FQualifiedFrameTime TrimTime, bool bTrimLeft, bool bDeleteKeys)
{
	SetFlags(RF_Transactional);

	if (TryModify())
	{
		if (bTrimLeft)
		{
			Params.StartOffset = HasStartFrame() ? GetStartOffsetAtTrimTime(TrimTime, Params, GetInclusiveStartFrame()) : 0;
		}

		Super::TrimSection(TrimTime, bTrimLeft, bDeleteKeys);
	}
}

UMovieSceneSection* UPaperZDMovieSceneAnimationSection::SplitSection(FQualifiedFrameTime SplitTime, bool bDeleteKeys)
{
	const float NewOffset = HasStartFrame() ? GetStartOffsetAtTrimTime(SplitTime, Params, GetInclusiveStartFrame()) : 0;

	UMovieSceneSection* NewSection = Super::SplitSection(SplitTime, bDeleteKeys);
	if (NewSection != nullptr)
	{
		UPaperZDMovieSceneAnimationSection* NewZDSection = Cast<UPaperZDMovieSceneAnimationSection>(NewSection);
		NewZDSection->Params.StartOffset = NewOffset;
	}
	return NewSection;
}

void UPaperZDMovieSceneAnimationSection::GetSnapTimes(TArray<FFrameNumber>& OutSnapTimes, bool bGetSectionBorders) const
{
	Super::GetSnapTimes(OutSnapTimes, bGetSectionBorders);

	const FFrameRate   FrameRate = GetTypedOuter<UMovieScene>()->GetTickResolution();
	const FFrameNumber StartFrame = GetInclusiveStartFrame();
	const FFrameNumber EndFrame = GetExclusiveEndFrame() - 1; // -1 because we don't need to add the end frame twice

	const float AnimPlayRate = FMath::IsNearlyZero(Params.PlayRate) ? 1.0f : Params.PlayRate;
	const float SeqLengthSeconds = (Params.GetSequenceLength() - (Params.StartOffset + Params.EndOffset)) / AnimPlayRate;

	FFrameTime SequenceFrameLength = SeqLengthSeconds * FrameRate;
	if (SequenceFrameLength.FrameNumber > 1)
	{
		// Snap to the repeat times
		FFrameTime CurrentTime = StartFrame;
		while (CurrentTime < EndFrame)
		{
			OutSnapTimes.Add(CurrentTime.FrameNumber);
			CurrentTime += SequenceFrameLength;
		}
	}
}

TOptional<FFrameTime> UPaperZDMovieSceneAnimationSection::GetOffsetTime() const
{
	return TOptional<FFrameTime>(Params.StartOffset * GetTypedOuter<UMovieScene>()->GetTickResolution());
}

#if WITH_EDITOR
void UPaperZDMovieSceneAnimationSection::PreEditChange(FProperty* PropertyAboutToChange)
{
	// Store the current play rate so that we can compute the amount to compensate the section end time when the play rate changes
	PreviousPlayRate = Params.PlayRate;
	
	Super::PreEditChange(PropertyAboutToChange);
}

void UPaperZDMovieSceneAnimationSection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Adjust the duration automatically if the play rate changes
	if (PropertyChangedEvent.Property != nullptr &&
		PropertyChangedEvent.Property->GetFName() == TEXT("PlayRate"))
	{
		float NewPlayRate = Params.PlayRate;

		if (!FMath::IsNearlyZero(NewPlayRate))
		{
			float CurrentDuration = UE::MovieScene::DiscreteSize(GetRange());
			float NewDuration = CurrentDuration * (PreviousPlayRate / NewPlayRate);
			SetEndFrame(GetInclusiveStartFrame() + FMath::FloorToInt(NewDuration));

			PreviousPlayRate = NewPlayRate;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

#undef LOCTEXT_NAMESPACE 
