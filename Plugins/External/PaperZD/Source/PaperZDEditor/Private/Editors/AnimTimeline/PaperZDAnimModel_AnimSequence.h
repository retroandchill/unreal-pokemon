// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "EditorUndoClient.h"
#include "UObject/UObjectGlobals.h"

class UPaperZDAnimSequence;
class FPaperZDAnimTimelineTrack_Notifies;
class FPaperZDAnimTimelineTrack_NotifiesPanel;
enum class EFrameNumberDisplayFormats : uint8;

/**
 * Abstraction that works as a bridge between the AnimTimeline and the animation assets.
 */
class FPaperZDAnimModel_AnimSequence : public FPaperZDAnimModel, public FEditorUndoClient
{
public:
	FPaperZDAnimModel_AnimSequence(const TSharedRef<FUICommandList>& InCommandList, UPaperZDAnimSequence* InAnimSequence, UPaperZDAnimationSource* InAnimSource, const TSharedPtr<FPaperZDAnimationSourceViewportClient>& InViewportClient);
	virtual ~FPaperZDAnimModel_AnimSequence();

	//~ Begin FPaperZDAnimModel Interface
	 virtual UPaperZDAnimSequence* GetAnimAssetBase() const override { return AnimSequence; }
	 virtual void UpdateRange() override;
	 virtual void RefreshTracks() override;
	 virtual void Initialize() override;
	 virtual void RefreshSnapTimes() override;
	//~ End FPaperZDAnimModel Interface

	//~Begin FEditorUndoClient Interface
	virtual void PostUndo( bool bSuccess );
	virtual void PostRedo( bool bSuccess );
	//~End FEditorUndoClient Interface

 	 const TSharedPtr<FPaperZDAnimTimelineTrack_Notifies>& GetNotifyRoot() const { return NotifyRoot; }

protected:
	/** Refresh notify tracks */
	void RefreshNotifyTracks();

 private:
 	/** UI handlers */
 	void SetDisplayFormat(EFrameNumberDisplayFormats InFormat);
 	bool IsDisplayFormatChecked(EFrameNumberDisplayFormats InFormat) const;
 	void ToggleDisplayPercentage();
 	bool IsDisplayPercentageChecked() const;

	//Property change handler
	typedef FCoreUObjectDelegates::FOnObjectPropertyChanged::FDelegate FOnPropertyChangedSignature;
	void OnPropertyChanged(UObject* ChangedObject, FPropertyChangedEvent& PropertyEvent);
 
 private:
	/* AnimSequence currently being represented by this model. */
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	/** Root track for notifies */
	TSharedPtr<FPaperZDAnimTimelineTrack_Notifies> NotifyRoot;

	/** Legacy notify panel track */
	TSharedPtr<FPaperZDAnimTimelineTrack_NotifiesPanel> NotifyPanel;
};
