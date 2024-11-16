// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDEditorSettings.generated.h"

//fwd declarations
enum class EFrameNumberDisplayFormats : uint8;

UENUM()
enum class EAnimSequencePlacementPolicy : uint8
{
	AlwaysAsk		UMETA(Tooltip = "Ask the user where to put the sequence"),
	SameFolder		UMETA(Tooltip = "Put the newly created sequence on the same folder as its parent AnimBP"),
	SubFolder		UMETA(Tooltip = "Put the newly created sequence on a subfolder with an specified name")
};

/**
 * Contains the Editor Only settings for PaperZD
 */
UCLASS(config=Editor, defaultconfig)
class UPaperZDEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UPaperZDEditorSettings();
	
	/* When creating a AnimSequence via the AnimBP Editor, where to put said AnimSequence. */
	UPROPERTY(EditAnywhere, config, Category = "Animation Blueprint")
	EAnimSequencePlacementPolicy SequencePlacementPolicy;

	/* Name of the folder in which to place the AnimSequences. */
	UPROPERTY(EditAnywhere, config, Category = "Animation Blueprint")
	FName SequencePlacementFolderName;

	/* Folder in which to place the notifies of the duplicated AnimBP. */
	UPROPERTY(EditAnywhere, config, Category = "Animation Blueprint")
	FName SequenceDuplicationFolderName;

	/* Color of the result node on the AnimGraph. */
	UPROPERTY(EditAnywhere, config, Category = "Animation Blueprint")
	FLinearColor ResultNodeTitleColor;

	/**
	 * Whether to automatically remove Custom AnimNotify functions that have been unregistered on the parent Animation Source.
	 * Disabling this will mark any "Receive Notify" function on the Animation Blueprints as "Unregistered" when needed, allowing later deletion.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Animation Blueprint")
	bool bAutomaticallyPurgeUnregisteredNotifyFunctions;

 	/* Colors to use on the time nodes for the montages. */ 
 	//UPROPERTY(EditAnywhere, config, Category = "Montages")  //@NOTE: Pending for montages 
 	FLinearColor SectionTimingNodeColor;
 
// 	UPROPERTY(EditAnywhere, config, Category = "Montages")
// 	FLinearColor NotifyTimingNodeColor;
// 
// 	UPROPERTY(EditAnywhere, config, Category = "Montages")
// 	FLinearColor BranchingPointTimingNodeColor;

// 	/** Snap value used to determine scrub resolution of the curve timeline */
// 	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
// 	float CurveEditorSnapInterval;

	/** Snap value used to determine scrub resolution of the anim timeline */
	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
	int32 TimelineScrubSnapValue;

	/** Display format for the anim timeline */
	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
	EFrameNumberDisplayFormats TimelineDisplayFormat;

	/** Whether to display percentage in the anim timeline */
	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
	bool bTimelineDisplayPercentage;
// 
// 	/** Whether to display keys in the timeline's curve tracks */
// 	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
// 	bool bTimelineDisplayCurveKeys;

	/** Whether to snap to various things */
	UPROPERTY(EditAnywhere, config, Category = "Timeline settings")
	TArray<FName> TimelineEnabledSnaps;

public:
	virtual bool CanEditChange(const FProperty* InProperty) const;
};
