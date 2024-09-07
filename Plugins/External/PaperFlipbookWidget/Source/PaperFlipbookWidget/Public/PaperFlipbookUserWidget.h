//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Misc/Attribute.h"
#include "Styling/SlateBrush.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "UObject/ScriptInterface.h"
#include "PaperFlipbookUserWidget.generated.h"


// Event for a non-looping flipbook finishing play
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlipbookFinishedPlayingEvent);

class SPaperFlipbookWidget;
class UPaperFlipbook;

/**
 * The paper flipbook widget allows you to display a flipbook asset in the UI.
 *
 * * No Children
 */
UCLASS(meta = (DisplayName = "Paper Flipbook"))
class PAPERFLIPBOOKWIDGET_API UPaperFlipbookUserWidget : public UWidget
{
	GENERATED_UCLASS_BODY()

public:

	/** Flipbook currently being played */
	UPROPERTY(Category = Appearance, EditAnywhere, meta = (DisplayThumbnail = "true"))
		UPaperFlipbook* SourceFlipbook;

	/** Are we currently playing . */
	UPROPERTY(Category = Appearance, EditAnywhere)
		uint8 bAutoPlay : 1;

	/** Current play rate of the flipbook */
	UPROPERTY(Category = Appearance, EditAnywhere)
		float PlayRate;

	/** Whether the flipbook should loop when it reaches the end, or stop */
	UPROPERTY(Category = Appearance, EditAnywhere)
		uint8 bLooping : 1;

	/** Color and opacity */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadOnly, meta = (sRGB = "true"))
		FLinearColor ColorAndOpacity;

	/** How to tile the image in Image mode */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<enum ESlateBrushTileType::Type> Tiling;

	/** How to mirror the image in Image mode.  This is normally only used for dynamic image brushes where the source texture
		comes from a hardware device such as a web camera. */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<enum ESlateBrushMirrorType::Type> Mirroring;

	/**  */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite)
		uint8 bOverrideContentSize : 1;

	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bOverrideContentSize"))
		FVector2D DesiredSize;


	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
		FGetLinearColor ColorAndOpacityDelegate;

public:

	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
		FOnPointerEvent OnMouseButtonDownEvent;

public:

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void SetOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void OverrideBrushSize(FVector2D InDesiredSize);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void SetBrushTintColor(FSlateColor TintColor);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void SetBrushTiling(TEnumAsByte<enum ESlateBrushTileType::Type> InTiling);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget|Appearance")
		void SetBrushMirroring(TEnumAsByte<enum ESlateBrushMirrorType::Type> InMirroring);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface


#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface
#endif

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

public:
	/** Event called whenever a non-looping flipbook finishes playing (either reaching the beginning or the end, depending on the play direction) */
	UPROPERTY(BlueprintAssignable, Category = "PaperFlipbook|Widget|Event")
		FOnFlipbookFinishedPlayingEvent OnFinishedPlaying;

public:
	/** Change the flipbook used by this instance (will reset the play time to 0 if it is a new flipbook). */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		virtual bool SetFlipbook(class UPaperFlipbook* NewFlipbook);

	/** Gets the flipbook used by this instance. */
	UFUNCTION(BlueprintPure, Category = "PaperFlipbook|Widget")
		virtual UPaperFlipbook* GetFlipbook();

	/** Start playback of flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void Play();

	/** Start playback of flipbook from the start */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void PlayFromStart();

	/** Start playback of flipbook in reverse */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void Reverse();

	/** Start playback of flipbook in reverse from the end */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void ReverseFromEnd();

	/** Stop playback of flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void Stop();

	/** Get whether this flipbook is playing or not. */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		bool IsPlaying() const;

	/** Get whether we are reversing or not */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		bool IsReversing() const;

	/** Jump to a position in the flipbook (expressed in frames). */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void SetPlaybackPositionInFrames(int32 NewFramePosition);

	/** Get the current playback position (in frames) of the flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		int32 GetPlaybackPositionInFrames() const;

	/** Jump to a position in the flipbook (expressed in seconds).  */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void SetPlaybackPosition(float NewPosition);

	/** Get the current playback position (in seconds) of the flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		float GetPlaybackPosition() const;

	/** true means we should loop, false means we should not. */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void SetLooping(bool bNewLooping);

	/** Get whether we are looping or not */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		bool IsLooping() const;

	/** Sets the new play rate for this flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void SetPlayRate(float NewRate);

	/** Get the current play rate for this flipbook */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		float GetPlayRate() const;

	/** Set the new playback position time to use */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		void SetNewTime(float NewTime);

	/** Get length of the flipbook (in seconds) */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		float GetFlipbookLength() const;

	/** Get length of the flipbook (in frames) */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		int32 GetFlipbookLengthInFrames() const;

	/** Get the nominal framerate that the flipbook will be played back at (ignoring PlayRate), in frames per second */
	UFUNCTION(BlueprintCallable, Category = "PaperFlipbook|Widget")
		float GetFlipbookFramerate() const;

protected:
	TSharedPtr<SPaperFlipbookWidget> MyFlipbookWG;

protected:
	void OnFlipbookFinishedPlaying();


protected:

	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
};
