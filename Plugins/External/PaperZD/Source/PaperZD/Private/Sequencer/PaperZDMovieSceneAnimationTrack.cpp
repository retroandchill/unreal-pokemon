// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Sequencer/PaperZDMovieSceneAnimationTrack.h"
#include "Sequencer/PaperZDMovieSceneAnimationSection.h"
#include "Sequencer/PaperZDMovieSceneAnimationTemplate.h"
#include "Compilation/MovieSceneCompilerRules.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "Compilation/IMovieSceneTemplateGenerator.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "MovieScene.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDMovieSceneAnimationTrack)
#endif

#define LOCTEXT_NAMESPACE "PaperZDMovieSceneAnimationTrack"

UPaperZDMovieSceneAnimationTrack::UPaperZDMovieSceneAnimationTrack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	TrackTint = FColor(124, 15, 124, 65);
#endif

	SupportedBlendTypes.Add(EMovieSceneBlendType::Absolute);
	EvalOptions.bEvaluateNearestSection_DEPRECATED = EvalOptions.bCanEvaluateNearestSection = true;
}

FMovieSceneEvalTemplatePtr UPaperZDMovieSceneAnimationTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FPaperZDMovieSceneAnimationTemplate(*CastChecked<UPaperZDMovieSceneAnimationSection>(&InSection));
}

void UPaperZDMovieSceneAnimationTrack::AddNewAnimationOnRow(FFrameNumber KeyTime, UPaperZDAnimSequence* AnimSequence, int32 RowIndex)
{
	UPaperZDMovieSceneAnimationSection* NewSection = Cast<UPaperZDMovieSceneAnimationSection>(CreateNewSection());
	{
		FFrameTime AnimationLength = AnimSequence->GetTotalDuration() * GetTypedOuter<UMovieScene>()->GetTickResolution();
		NewSection->InitialPlacementOnRow(AnimationSections, KeyTime, AnimationLength.FrameNumber.Value, RowIndex);
		NewSection->Params.Animation = AnimSequence;
	}

	AddSection(*NewSection);
}


TArray<UMovieSceneSection*> UPaperZDMovieSceneAnimationTrack::GetAnimSectionsAtTime(FFrameNumber Time)
{
	TArray<UMovieSceneSection*> Sections;
	for (auto Section : AnimationSections)
	{
		if (Section->IsTimeWithinSection(Time))
		{
			Sections.Add(Section);
		}
	}

	return Sections;
}


const TArray<UMovieSceneSection*>& UPaperZDMovieSceneAnimationTrack::GetAllSections() const
{
	return AnimationSections;
}


bool UPaperZDMovieSceneAnimationTrack::SupportsMultipleRows() const
{
	return false;
}

bool UPaperZDMovieSceneAnimationTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass == UPaperZDMovieSceneAnimationSection::StaticClass();
}


UMovieSceneSection* UPaperZDMovieSceneAnimationTrack::CreateNewSection()
{
	return NewObject<UPaperZDMovieSceneAnimationSection>(this);
}


void UPaperZDMovieSceneAnimationTrack::RemoveAllAnimationData()
{
	AnimationSections.Empty();
}


bool UPaperZDMovieSceneAnimationTrack::HasSection(const UMovieSceneSection& Section) const
{
	return AnimationSections.Contains(&Section);
}


void UPaperZDMovieSceneAnimationTrack::AddSection(UMovieSceneSection& Section)
{
	AnimationSections.Add(&Section);
}


void UPaperZDMovieSceneAnimationTrack::RemoveSection(UMovieSceneSection& Section)
{
	AnimationSections.Remove(&Section);
}


bool UPaperZDMovieSceneAnimationTrack::IsEmpty() const
{
	return AnimationSections.Num() == 0;
}

#if WITH_EDITORONLY_DATA

FText UPaperZDMovieSceneAnimationTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("TrackName", "Animation");
}

#endif

FMovieSceneTrackRowSegmentBlenderPtr UPaperZDMovieSceneAnimationTrack::GetRowSegmentBlender() const
{
	// Apply an upper bound exclusive blend
	struct FPaperZDAnimationRowCompilerRules : FMovieSceneTrackRowSegmentBlender
	{
		bool bUseLegacySectionIndexBlend;
		FPaperZDAnimationRowCompilerRules(bool bInUseLegacySectionIndexBlend) : bUseLegacySectionIndexBlend(bInUseLegacySectionIndexBlend) {}

		virtual void Blend(FSegmentBlendData& BlendData) const override
		{
			// Run the default high pass filter for overlap priority
			MovieSceneSegmentCompiler::FilterOutUnderlappingSections(BlendData);

			if (bUseLegacySectionIndexBlend)
			{
				// Weed out based on array index (legacy behaviour)
				MovieSceneSegmentCompiler::BlendSegmentLegacySectionOrder(BlendData);
			}
		}
	};
	return FPaperZDAnimationRowCompilerRules(/*bUseLegacySectionIndexBlend*/ false);
}
#undef LOCTEXT_NAMESPACE
