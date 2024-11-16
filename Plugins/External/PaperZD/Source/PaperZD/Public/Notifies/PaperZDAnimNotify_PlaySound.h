// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Notifies/PaperZDAnimNotify.h"
#include "PaperZDAnimNotify_PlaySound.generated.h"

class USoundBase;
UCLASS(Blueprintable, Config = Game, meta=(DisplayName="Play Sound"))
class PAPERZD_API UPaperZDAnimNotify_PlaySound : public UPaperZDAnimNotify
{
	GENERATED_UCLASS_BODY()

public:
	// Sound to Play
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	TObjectPtr<USoundBase> Sound;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float VolumeMultiplier;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float PitchMultiplier;

	// If this sound should follow its owner
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	uint32 bFollow : 1;
	
#if WITH_EDITORONLY_DATA
	// Should ignore attenuation in the editor and play as a 2D sound?
	UPROPERTY(Config, EditAnywhere, Category = "AnimNotify")
	uint32 bPreviewIgnoreAttenuation:1;
#endif

	// Socket or bone name to attach sound to
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (EditCondition = "bFollow"))
	FName AttachName;

public:
	void OnReceiveNotify_Implementation(UPaperZDAnimInstance *OwningInstance = nullptr) const override;
	FName GetDisplayName_Implementation() const override;

};
