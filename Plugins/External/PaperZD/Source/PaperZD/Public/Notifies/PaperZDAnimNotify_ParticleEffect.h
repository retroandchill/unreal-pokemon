// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Notifies/PaperZDAnimNotify.h"
#include "PaperZDAnimNotify_ParticleEffect.generated.h"

class UParticleSystem;

/**
 * Spawns a one shot particle effect on a given location around the RenderComponent.
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Play Particle Effect"))
class PAPERZD_API UPaperZDAnimNotify_ParticleEffect : public UPaperZDAnimNotify
{
	GENERATED_BODY()

	// Cached version of the Rotation Offset already in Quat form
	FQuat RotationOffsetQuat;

public:	
	// Particle System to Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(DisplayName="Particle System"))
	TObjectPtr<UParticleSystem> PSTemplate;

	// Location offset from the socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FVector LocationOffset;

	// Rotation offset from socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FRotator RotationOffset;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	FVector Scale;

	// Should attach to the bone/socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bAttached;

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName SocketName;


public:
	UPaperZDAnimNotify_ParticleEffect();

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject interface

	//Override the native notify implementation
	void OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance = nullptr) const override;
	FName GetDisplayName_Implementation() const override;
	
	
};
