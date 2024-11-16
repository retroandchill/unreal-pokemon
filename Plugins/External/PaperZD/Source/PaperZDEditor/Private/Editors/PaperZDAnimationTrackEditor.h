// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"
#include "Misc/Guid.h"
#include "ISequencer.h"
#include "MovieSceneTrack.h"
#include "ISequencerSection.h"
#include "ISequencerTrackEditor.h"
#include "MovieSceneTrackEditor.h"
#include "ContentBrowserDelegates.h"

//Forward Declarations
class FSequencerSectionPainter;
class UPaperZDAnimSequence;
class IPaperZDSequencerSource;

/**
 * Tools for adding PaperZD related tracks to sequencer.
 */
class FPaperZDAnimationTrackEditor : public FMovieSceneTrackEditor
{
public:
	//Constructor	
	FPaperZDAnimationTrackEditor(TSharedRef<ISequencer> OwningSequencer);

	/**
	* Creates an instance of this class.  Called by a sequencer
	*
	* @param OwningSequencer The sequencer instance to be used by this tool
	* @return The new instance of this class
	*/
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

	//~Begin ISequencerTrackEditor interface
	bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual void BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass) override;
	//~End ISequencerTrackEditor interface
	
public:
	void AddAnimationSubMenu(FMenuBuilder& MenuBuilder, FGuid ObjectBinding,  UMovieSceneTrack* Track);
	void AddAnimationSubMenu(FMenuBuilder& MenuBuilder, FGuid ObjectBinding, class UPaperZDMovieSceneAnimationSection *Section);

private:
	/* Creates the widget for displaying the list of animations of the given sequencer source object. */
	TSharedRef<SWidget> CreateAnimationListWidget(IPaperZDSequencerSource* SequencerSource, FOnAssetSelected OnAssetSelected);

	/** Delegate for AnimatablePropertyChanged in AddKey */
	FKeyPropertyResult AddKeyInternal(FFrameNumber KeyTime, UObject* Object, class UPaperZDAnimSequence* AnimSequence, UMovieSceneTrack* Track, int32 RowIndex);

	/** Animation asset selected */
	void OnAnimationSequenceSelected(const FAssetData& AssetData, FGuid ObjectBinding, UMovieSceneTrack* Track);
	void OnAnimationSequenceSelected(const FAssetData& AssetData, FGuid ObjectBinding, class UPaperZDMovieSceneAnimationSection* Section);
};

/**
 * A section that comprises a single animation to be displayed.
 */
class FPaperZDAnimationSection : public ISequencerSection, public TSharedFromThis<FPaperZDAnimationSection>
{
public:
	/** Constructor. */
	FPaperZDAnimationSection(UMovieSceneSection& InSection, FPaperZDAnimationTrackEditor *TrackEditor);

	/** Virtual destructor. */
	virtual ~FPaperZDAnimationSection() { }

	//~Begin ISequencerSection interface
	virtual UMovieSceneSection* GetSectionObject() override;
	virtual FText GetSectionTitle() const override;
	virtual float GetSectionHeight() const override;
	virtual int32 OnPaintSection(FSequencerSectionPainter& Painter) const override;
	virtual void BeginResizeSection() override;
	virtual void ResizeSection(ESequencerSectionResizeMode ResizeMode, FFrameNumber ResizeTime) override;
	virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override;
	//~End ISequencerSection interface

private:
	/* The runtime section. */
	class UPaperZDMovieSceneAnimationSection& Section;
	
	/* The track editor that owns this. */
	FPaperZDAnimationTrackEditor* TrackEditor;

	/** Cached start offset value valid only during resize */
	float InitialStartOffsetDuringResize;

	/** Cached start time valid only during resize */
	FFrameNumber InitialStartTimeDuringResize;
};
