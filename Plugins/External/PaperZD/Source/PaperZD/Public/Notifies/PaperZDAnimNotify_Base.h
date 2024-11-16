// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Components/PrimitiveComponent.h"
#include "PaperZDAnimNotify_Base.generated.h"

class UPaperZDAnimSequence;
class UPaperZDAnimInstance;

/**
 * Base class for all the plugin's notifies.
 */
UCLASS(abstract, hidecategories = UObject, collapsecategories)
class PAPERZD_API UPaperZDAnimNotify_Base : public UObject
{
	GENERATED_UCLASS_BODY()

	/* Playback time that triggers this event. */
	UPROPERTY()
	float Time;

	/* Friendly name for the notify. */
	UPROPERTY()
	FName Name;

	/* Color of the Notify. */
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	FLinearColor Color;

	/* Index of the track in which the notify is stored. */
	UPROPERTY()
	int32 TrackIndex;

	/* The render component currently used by the AnimSequence that owns this notify. */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "AnimNotify")
	mutable TWeakObjectPtr<UPrimitiveComponent> SequenceRenderComponent;

#if WITH_EDITORONLY_DATA
	/** Whether this notify instance should fire in animation editors */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="AnimNotify")
	bool bShouldFireInEditor;
#endif

public:
	/**
	* We need the GetWorld defined, if not, the collision or trace functions won't be available in blueprints.
	* For this to work, the world must be correctly defined when TickNotify does get called, the world can be obtained from RenderComponent used for the AnimSequence
	*/
	virtual class UWorld* GetWorld() const override;

	/**
	 * Called each Tick to process the notify and trigger it when necessary 
	 * Playtime and PreviousPlaytime are given for convenience, because the animation can loop.
	 * 
	 * The notify can be made 'active' by changing its 'bPersistentActiveState' variable to true, which will persist on the next update cycle, by adding it to the list of 'active notifies' on the player.
	 */
	virtual void TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance = nullptr) const;

	/**
	 * Obtain the name to be displayed on the editor's detail's panel
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Notify")
	FName GetDisplayName() const;

	/* Called when this notify was active and gets 'aborted', usually by the owner AnimSequence no longer being updated. */
	virtual void OnNotifyAborted(UPaperZDAnimInstance* OwningInstance) const {}

#if WITH_EDITOR
	virtual bool CanBePlaced(UPaperZDAnimSequence* Animation) const { return true; }
#endif

protected:
	/* Obtain the asset that contains this notify instance.*/
	UObject* GetContainingAsset() const;
};
