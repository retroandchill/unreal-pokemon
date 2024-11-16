// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Curves/KeyHandle.h"
#include "MovieSceneSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "PaperZDMovieSceneAnimationSection.generated.h"

class UPaperZDMovieSceneAnimationSection;
class UPaperZDAnimSequence;

/**
 * Parameters for the Animation Section
 */
USTRUCT()
struct PAPERZD_API FPaperZDMovieSceneAnimationParams
{
	GENERATED_BODY()

	FPaperZDMovieSceneAnimationParams();

	/** Gets the animation duration, modified by play rate */
	float GetDuration() const;

	/** Gets the animation sequence length, not modified by play rate */
	float GetSequenceLength() const;
	
	/** The offset into the beginning of the animation clip */
	UPROPERTY(EditAnywhere, Category = "Animation")
	float StartOffset;

	/** The offset into the end of the animation clip */
	UPROPERTY(EditAnywhere, Category = "Animation")
	float EndOffset;

	/** The playback rate of the animation clip */
	UPROPERTY(EditAnywhere, Category = "Animation")
	float PlayRate;

	/** Reverse the playback of the animation clip */
	UPROPERTY(EditAnywhere, Category = "Animation")
	uint32 bReverse : 1;

	/** Name of the slot that the animation will be overriding on the AnimationBlueprint. */
	UPROPERTY(EditAnywhere, Category = "Animation")
	FName SlotName;

	/** The weight curve for this animation section */
	UPROPERTY(EditAnywhere, Category = "Animation")
	FMovieSceneFloatChannel Weight;

	/* Animation sequence to use. */
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UPaperZDAnimSequence> Animation;
};

/**
* Movie scene section that controls PaperZD Sequence animations
*/
UCLASS(MinimalAPI)
class UPaperZDMovieSceneAnimationSection : public UMovieSceneSection
{
	GENERATED_UCLASS_BODY()

public:
	
	/* Parameters that define the section. */
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (ShowOnlyInnerProperties))
	FPaperZDMovieSceneAnimationParams Params;

public:
	//~ MovieSceneSection interface
	virtual TOptional<TRange<FFrameNumber>> GetAutoSizeRange() const override;
	virtual void TrimSection(FQualifiedFrameTime TrimTime, bool bTrimLeft, bool bDeleteKeys) override;
	virtual UMovieSceneSection* SplitSection(FQualifiedFrameTime SplitTime, bool bDeleteKeys) override;
	virtual void GetSnapTimes(TArray<FFrameNumber>& OutSnapTimes, bool bGetSectionBorders) const override;
	virtual TOptional<FFrameTime> GetOffsetTime() const override;

private:
	float GetStartOffsetAtTrimTime(FQualifiedFrameTime TrimTime, const FPaperZDMovieSceneAnimationParams& Params, FFrameNumber StartFrame);

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	float PreviousPlayRate;
#endif
};
