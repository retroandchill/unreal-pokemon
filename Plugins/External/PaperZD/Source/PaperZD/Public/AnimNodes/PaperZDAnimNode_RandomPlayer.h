// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "AnimNodes/PaperZDAnimNode_Base.h"
#include "PaperZDAnimNode_RandomPlayer.generated.h"

class UPaperZDAnimSequence;

/**
 * Defines an entry for the random sequence player.
 */
USTRUCT()
struct FPaperZDRandomPlayerEntry
{
	GENERATED_BODY()

	/* Animation sequence to play. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	/* Chance that this entry will play when not in shuffle mode (normalized against others). */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (UIMin = "0.0", ClampMin = "0.0", UIMax = "1.0", ClampMax = "1.0"))
	float ChanceToPlay;

	/* Minimum amount of loops that the sequence needs to play before jumping to the next entry. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (UIMin = "0.0", ClampMin = "0.0"))
	int32 MinLoopCount;

	/* Maximum amount of loops that the sequence needs to play before jumping to the next entry. */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (UIMin = "0.0", ClampMin = "0.0"))
	int32 MaxLoopCount;

	/* Minimum playrate for this sequence. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	float MinPlayRate;

	/* Maximum playrate for this sequence. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxPlayRate;

	FPaperZDRandomPlayerEntry()
		: AnimSequence(nullptr)
		, ChanceToPlay(1.0f)
		, MinLoopCount(1)
		, MaxLoopCount(1)
		, MinPlayRate(1.0f)
		, MaxPlayRate(1.0f)
	{}
};

/**
 * Plays sequences randomly by choosing from a provided list.
 */
USTRUCT()
struct PAPERZD_API FPaperZDAnimNode_RandomPlayer : public FPaperZDAnimNode_Base
{
	GENERATED_BODY()

	/* Animation sequence to play. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<FPaperZDRandomPlayerEntry> Entries;	

	/* If true, the player will create a shuffle list that will ensure each entry gets played at least once before looping. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bShuffleMode;

	/* The internal playback time. */
	float PlaybackTime;

	/* Index to the current entry being played. */
	int32 CurrentEntryIdx;

	/* Remaining loops to play for the current sequence being played. */
	int32 RemainingLoops;

	/* Selected playrate for the animation. */
	float PlayRate;

	/**
	 * A list with the indices of the entries ordered by decreasing play chance when not in shuffle mode.
	 * In shuffle mode it contains the order in which to play the entries (already shuffled).
	 */
	TArray<int32> OrderedList;

	/* Current index of the shuffle list. */
	int32 ShuffleIndex;

	/* The sum of all the play chances. */
	float AggregatedChance;

public:
	//ctor
	FPaperZDAnimNode_RandomPlayer();

	//~Begin FPaperZDAnimNode_Base Interface
	virtual void OnInitialize(const FPaperZDAnimationInitContext& InitContext) override;
	virtual void OnUpdate(const FPaperZDAnimationUpdateContext& UpdateContext) override;
	virtual void OnEvaluate(FPaperZDAnimationPlaybackData& OutData) override;
	//~End FPaperZDAnimNode_Base Interface

private:
	/* Updates the index list. */
	void GenerateOrderedList();

	/* Obtains the next entry to play (either by randomly choosing one, or by popping one from the shuffle list). */
	void PickNextEntry();
};
