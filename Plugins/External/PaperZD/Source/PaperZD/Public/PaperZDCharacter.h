// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "PaperCharacter.h"
#include "Sequencer/IPaperZDSequencerSource.h"
#include "PaperZDCharacter.generated.h"

class UPaperZDAnimationComponent;
class UPaperZDAnimInstance;
class UPaperZDAnimBP;

/**
 * A PaperZD character inherits from the Paper2D character and sets up the animation blueprint to manage its main flipbook component.
 */
UCLASS(ClassGroup = (PaperZD))
class PAPERZD_API APaperZDCharacter : public APaperCharacter, public IPaperZDSequencerSource
{
	GENERATED_UCLASS_BODY()

private:
	/* Animation component that manages the AnimBP. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PaperZD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;

	/* The AnimInstance that drives this character, maintained for backwards compatibility. */
	UPROPERTY(Transient, Category = "PaperZD", BlueprintGetter = "GetAnimInstance")
	TObjectPtr<UPaperZDAnimInstance> AnimInstance;

#if WITH_EDITORONLY_DATA
	/* Deprecated: AnimationBlueprint used for animating the character, required for backwards support. */
 	UPROPERTY()
 	TObjectPtr<UPaperZDAnimBP> AnimationBlueprint_DEPRECATED;
#endif

public:

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	//~ End AActor Interface

	//~ Begin IPaperZDSequencerSource Interface
	virtual TSubclassOf<UPaperZDAnimInstance> GetSequencerAnimInstanceClass() const override;
	virtual UPaperZDAnimInstance* GetSequencerAnimInstance() override;
	//~ End IPaperZDSequencerSource Interface

	//getters
	FORCEINLINE UPaperZDAnimationComponent* GetAnimationComponent() const { return AnimationComponent; }

	/* Obtain the AnimInstance that drives this character. */
	UFUNCTION(BlueprintPure, Category = "PaperZD")
	UPaperZDAnimInstance* GetAnimInstance() const;
};
