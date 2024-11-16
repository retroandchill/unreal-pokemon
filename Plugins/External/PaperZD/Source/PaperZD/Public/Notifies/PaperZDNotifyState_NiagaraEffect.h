// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Notifies/PaperZDAnimNotifyState.h"
#include "PaperZDNotifyState_NiagaraEffect.generated.h"

class UNiagaraSystem;

/**
 * Spawns a one shot particle effect on a given location around the RenderComponent.
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Timed Niagara Effect"))
class PAPERZD_API UPaperZDNotifyState_NiagaraEffect : public UPaperZDAnimNotifyState
{
	GENERATED_BODY()

	// Cached version of the Rotation Offset already in Quat form
	FQuat RotationOffsetQuat;

public:	
	// Particle System to Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(DisplayName="Niagara System"))
	TObjectPtr<UNiagaraSystem> PSTemplate;

	// Location offset from the socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FVector LocationOffset;

	// Rotation offset from socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FRotator RotationOffset;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, Category="AnimNotify")
	FVector Scale;

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName SocketName;

	// Whether or not we destroy the component at the end of the notify or instead just stop
	// the emitters.
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (DisplayName = "Destroy Immediately", ToolTip = "Whether the particle system should be immediately destroyed at the end of the notify state or be allowed to finish"))
	bool bDestroyAtEnd;

public:
	//ctor
	UPaperZDNotifyState_NiagaraEffect();

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject interface

	//Override the native notify implementation
	virtual void OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual void OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const override;
	virtual FName GetDisplayName_Implementation() const override;

private:
	/* Tag to use to recognize the effects that get spawned by this notify state. */
	FORCEINLINE FName GetSpawnedComponentTag()const { return GetFName(); }
};
