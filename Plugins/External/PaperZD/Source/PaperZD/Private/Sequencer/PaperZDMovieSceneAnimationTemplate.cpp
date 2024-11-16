// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Sequencer/PaperZDMovieSceneAnimationTemplate.h"
#include "Sequencer/PaperZDMovieSceneAnimationSection.h"
#include "Sequencer/IPaperZDSequencerSource.h"
#include "AnimSequences/Players/PaperZDAnimationPlaybackData.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"
#include "PaperZDAnimInstance.h"
#include "IMovieScenePlayer.h"
#include "Engine/World.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDMovieSceneAnimationTemplate)
#endif

namespace FPaperZDMovieSceneHelpers
{
	bool ShouldUsePreviewPlayback(IMovieScenePlayer& Player, UObject& RuntimeObject)
	{
		// we also use PreviewSetAnimPosition in PIE when not playing, as we can preview in PIE
		bool bIsNotInPIEOrNotPlaying = (RuntimeObject.GetWorld() && !RuntimeObject.GetWorld()->HasBegunPlay()) || Player.GetPlaybackStatus() != EMovieScenePlayerStatus::Playing;
		return GIsEditor && bIsNotInPIEOrNotPlaying;
	}
}

//////////////////////////////////////////////////////////////////////////
//// FPaperZDMovieSceneAnimationSectionTemplateParams
//////////////////////////////////////////////////////////////////////////

float FPaperZDMovieSceneAnimationSectionTemplateParams::MapTimeToAnimation(FFrameTime InPosition, FFrameRate InFrameRate, UPaperZDAnimSequence* Sequence) const
{
	InPosition = FMath::Clamp(InPosition, FFrameTime(SectionStartTime), FFrameTime(SectionEndTime - 1));

	const float SectionPlayRate = PlayRate;
	const float AnimPlayRate = FMath::IsNearlyZero(SectionPlayRate) ? 1.0f : SectionPlayRate;

	const float SeqLength = GetSequenceLength() - (StartOffset + EndOffset);

	float AnimPosition = FFrameTime::FromDecimal((InPosition - SectionStartTime).AsDecimal() * AnimPlayRate) / InFrameRate;
	if (SeqLength > 0.f)
	{
		AnimPosition = FMath::Fmod(AnimPosition, SeqLength);
	}
	AnimPosition += StartOffset;
	if (bReverse)
	{
		AnimPosition = (SeqLength - (AnimPosition - StartOffset)) + StartOffset;
	}

	return AnimPosition;
}

//////////////////////////////////////////////////////////////////////////
//// FPaperZDMovieSceneAnimationTemplate
//////////////////////////////////////////////////////////////////////////
// Token producer, used for preparing and restoring pre-animated state.
struct FPaperZDPreAnimatedAnimationTokenProducer : IMovieScenePreAnimatedTokenProducer
{
	virtual IMovieScenePreAnimatedTokenPtr CacheExistingState(UObject& Object) const
	{
		struct FToken : IMovieScenePreAnimatedToken
		{
			FToken(UPaperZDAnimInstance* AnimInstance)
			{
				AnimInstance->PrepareForMovieSequence();
			}

			virtual void RestoreState(UObject& Object, const UE::MovieScene::FRestoreStateParams& Params)
			{
				CastChecked<UPaperZDAnimInstance>(&Object)->RestorePreMovieSequenceState();
			}
		};

		return FToken(CastChecked<UPaperZDAnimInstance>(&Object));
	}
};

//Actuator, used for gathering all the animation data and spitting out the ZD Animation Data to the player
namespace ZD
{
	//Parameters to be passed to the actuator
	struct FMinimalAnimParameters
	{
		FMinimalAnimParameters(UPaperZDAnimSequence* InSequence, float InPreviousEvalTime, float InEvalTime, float InWeight, FName InSlotName, uint32 InSectionId)
			: SequencePtr(InSequence)
			, PreviousEvalTime(InPreviousEvalTime)
			, EvalTime(InEvalTime)
			, Weight(InWeight)
			, SlotName(InSlotName)
			, SectionId(InSectionId)
		{}

		TWeakObjectPtr<UPaperZDAnimSequence> SequencePtr;
		float PreviousEvalTime;
		float EvalTime;
		float Weight;
		FName SlotName;
		uint32 SectionId;
	};

	//Represents the collection of animations to be blended onto the final playback data
	struct FBlendedAnimation
	{
		TArray<FMinimalAnimParameters> Animations;

		FBlendedAnimation& Resolve(TMovieSceneInitialValueStore<FBlendedAnimation>& InitialValueStore)
		{
			return *this;
		}
	};

	//Actuator which mixes the animation onto the final playback data
	struct FPaperZDAnimationActuator : TMovieSceneBlendingActuator<FBlendedAnimation>
	{
		//ctor
		FPaperZDAnimationActuator()
			: TMovieSceneBlendingActuator<FBlendedAnimation>(GetActuatorTypeID())
		{}

		/* Obtain the ID of this actuator, used to identify it when registering a blend token. */
		static FMovieSceneBlendingActuatorID GetActuatorTypeID()
		{
			static FMovieSceneAnimTypeID TypeID = TMovieSceneAnimTypeID<FPaperZDAnimationActuator, 0>();
			return FMovieSceneBlendingActuatorID(TypeID);
		}

		static FMovieSceneAnimTypeID GetAnimControlTypeID()
		{
			static FMovieSceneAnimTypeID TypeID = TMovieSceneAnimTypeID<FPaperZDAnimationActuator, 2>();
			return TypeID;
		}

		/* Required, but shouldn't ever get here. */
		virtual FBlendedAnimation RetrieveCurrentValue(UObject* InObject, IMovieScenePlayer* Player) const override
		{
			check(false);
			return FBlendedAnimation();
		}

		/* Apply the animation data. */
		virtual void Actuate(UObject* InObject, const FBlendedAnimation& InFinalValue, const TBlendableTokenStack<FBlendedAnimation>& OriginalStack, const FMovieSceneContext& Context, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
		{
			//The object should always conform to the sequencer source interface
			IPaperZDSequencerSource* SequencerSource = CastChecked<IPaperZDSequencerSource>(InObject);
			UPaperZDAnimInstance* AnimInstance = SequencerSource->GetSequencerAnimInstance();
			if (AnimInstance)
			{
				//Store the pre-animated state
				OriginalStack.SavePreAnimatedState(Player, *AnimInstance, GetAnimControlTypeID(), FPaperZDPreAnimatedAnimationTokenProducer());

				//Setup flags
 				const bool bPreviewPlayback = FPaperZDMovieSceneHelpers::ShouldUsePreviewPlayback(Player, *AnimInstance);
 				const EMovieScenePlayerStatus::Type PlayerStatus = Player.GetPlaybackStatus();

				// If the playback status is jumping, ie. one such occurrence is setting the time for thumbnail generation, disable anim notifies updates because it could fire audio
 				const bool bFireNotifies = !bPreviewPlayback || (PlayerStatus != EMovieScenePlayerStatus::Jumping && PlayerStatus != EMovieScenePlayerStatus::Stopped);
				
				//Start collecting the animations into the final playback data structure
				TMap<FName, FPaperZDAnimationPlaybackData> PlaybackDataMap;
				for (const FMinimalAnimParameters& Params : InFinalValue.Animations)
				{
					//Process the notifies if needed
					if (bFireNotifies)
					{
						const float DeltaTime = Params.EvalTime - Params.PreviousEvalTime;
						AnimInstance->GetPlayer()->ProcessAnimSequenceNotifies(Params.SequencePtr.Get(), DeltaTime, Params.EvalTime, Params.PreviousEvalTime, Params.Weight, AnimInstance);
					}

					//Obtain the playback handle for the slot
					FPaperZDAnimationPlaybackData& PlaybackData = PlaybackDataMap.FindOrAdd(Params.SlotName);
					PlaybackData.AddAnimation(Params.SequencePtr.Get(), Params.EvalTime, Params.Weight);
				}

				//Now that we have all the animation pertaining to each slot, apply to the AnimInstance
				//we want Sequencer animations to have more priority than any AnimationOverride the user might have triggered so we will override them if needed
				for (TPair<FName, FPaperZDAnimationPlaybackData> KVPair : PlaybackDataMap)
				{
					AnimInstance->SetAnimationOverrideDataBySlot(KVPair.Key, KVPair.Value, true);
				}

				//If we're on a preview playback, we need to force the animation blueprint to do a 'virtual tick' so we can process the animations and render them correctly
				if (bPreviewPlayback)
				{
					AnimInstance->UpdateSequencerPreview();
				}
			}
		}
	};

	void BlendValue(FBlendedAnimation& OutBlend, const FMinimalAnimParameters& InValue, float Weight, EMovieSceneBlendType BlendType, TMovieSceneInitialValueStore<FBlendedAnimation>& InitialValueStore)
	{
		OutBlend.Animations.Add(InValue);
	}
}

//Required template specializations
template<>
FMovieSceneAnimTypeID GetBlendingDataType<ZD::FBlendedAnimation>()
{
	static FMovieSceneAnimTypeID TypeID = FMovieSceneAnimTypeID::Unique();
	return TypeID;
}

FPaperZDMovieSceneAnimationTemplate::FPaperZDMovieSceneAnimationTemplate(const UPaperZDMovieSceneAnimationSection& InSection)
	: Params(InSection.Params, InSection.GetInclusiveStartFrame(), InSection.GetExclusiveEndFrame())
{}

void FPaperZDMovieSceneAnimationTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Ensure the accumulator knows how to actually apply component transforms
	FMovieSceneBlendingActuatorID ActuatorTypeID = ZD::FPaperZDAnimationActuator::GetActuatorTypeID();
	FMovieSceneBlendingAccumulator& Accumulator = ExecutionTokens.GetBlendingAccumulator();
	if (!Accumulator.FindActuator<ZD::FBlendedAnimation>(ActuatorTypeID))
	{
		Accumulator.DefineActuator(ActuatorTypeID, MakeShared<ZD::FPaperZDAnimationActuator>());
	}

	// calculate the time at which to evaluate the animation
	float EvalTime = Params.MapTimeToAnimation(Context.GetTime(),Context.GetFrameRate(), Params.Animation);
	float PreviousEvalTime = Params.MapTimeToAnimation(Context.GetPreviousTime(), Context.GetFrameRate(), Params.Animation);

	float ManualWeight = 1.f;
	Params.Weight.Evaluate(Context.GetTime(), ManualWeight);
	const float Weight = ManualWeight * EvaluateEasing(Context.GetTime());
		
	// Create the execution token with the minimal AnimParams
	ZD::FMinimalAnimParameters AnimParams(
		Params.Animation, PreviousEvalTime, EvalTime, Weight, Params.SlotName, GetTypeHash(PersistentData.GetSectionKey())
	);

	FOptionalMovieSceneBlendType BlendType = GetSourceSection()->GetBlendType();
	ExecutionTokens.BlendToken(ActuatorTypeID, TBlendableToken<ZD::FBlendedAnimation>(AnimParams, BlendType.Get(), 1.0f));
}
