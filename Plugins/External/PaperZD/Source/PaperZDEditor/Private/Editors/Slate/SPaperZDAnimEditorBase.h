// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Editors/Util/PaperZDEditorDelegates.h"

class FPaperZDAnimationSourceEditor;
class UPaperZDAnimSequence;
class FPaperZDAnimModel;
class SVerticalBox;

/**
 * An editor for the animation sequences (track and notifies).
 */
class SPaperZDAnimEditorBase : public SCompoundWidget
{
	/* Source editor hosting app. */
	TWeakPtr<FPaperZDAnimationSourceEditor> SourceEditorPtr;

	/* Called when objects are selected. */
	FPaperZDEditorDelegates::OnObjectsSelectedSignature OnObjectsSelected;

public:
	SLATE_BEGIN_ARGS(SPaperZDAnimEditorBase)
	{}
	SLATE_EVENT(FPaperZDEditorDelegates::OnObjectsSelectedSignature, OnObjectsSelected)
	SLATE_ARGUMENT(TSharedPtr<FPaperZDAnimModel>, AnimModel)
	SLATE_END_ARGS()

	/* Constructs the widget. */
	void Construct(const FArguments& InArgs, TSharedPtr<FPaperZDAnimationSourceEditor> InSourceEditor);

	/** Accessors to the current viewed Min/Max input range of the editor */
	float GetViewMinInput() const { return ViewMinInput; }
	float GetViewMaxInput() const { return ViewMaxInput; }

	/** Accessors to the total Min/Max input range of the current object being edited */
	float GetMinInput() const { return 0.f; }
	float GetMaxInput() const { return GetSequenceLength(); }

	void SetInputViewRange(float InViewMinInput, float InViewMaxInput);

	//@NOTE: NOT USED?
// 	/** Scrolls the input view based on the supplied ScreenDelta */
// 	void ScrollInputViewRange(float ScreenDelta, FVector2D ScreenViewSize);

	/** Delegate when selection changed **/
	void OnSelectionChanged(const TArray<UObject*>& SelectedItems);
// 
// 	/** Get Current Scrub Value **/
// 	float GetScrubValue() const;
// 
// 	/** Creates an editor object from the given type to be used in a details panel */
// 	UObject* ShowInDetailsView(UClass* EdClass);
// 
// 	/** Clears the detail view of whatever we displayed last */
// 	void ClearDetailsView();
// 
// 	// FGCObject interface start
// 	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
// 	// FGCObject interface end
// 
// 	/** Creates a anchor widget for each animation editor type */
// 	virtual TSharedRef<SWidget> CreateDocumentAnchor();
// 
// protected:
// 	/** Preview scene reference **/
// 	TWeakPtr<IPersonaPreviewScene> PreviewScenePtr;
// 
// 	TSharedRef<IPersonaPreviewScene> GetPreviewScene() const { return PreviewScenePtr.Pin().ToSharedRef(); }
// 
// 	/** Allows derived classes to init newly created editor objects */
// 	virtual void InitDetailsViewEditorObject(class UEditorAnimBaseObj* EdObj) {};
// 
	/** Get the length of the current sequence */
	float GetSequenceLength() const;

	/** Get the sequence that is currently being edited */
	virtual UPaperZDAnimSequence* GetEditorObject() const = 0;
// 
// 	/** Get Name of Object being edited **/
// 	FText GetEditorObjectName() const;

	/** Get Min/Max Input of value **/
	float ViewMinInput;
	float ViewMaxInput;
// 
// 	/** To get scrub value, get preview instance **/
// 	class UAnimSingleNodeInstance* GetPreviewInstance() const;
// 
// 	/** For the information section **/
// 	FText GetCurrentSequenceTime() const;
// 	FText GetCurrentPercentage() const;
// 	FText GetCurrentFrame() const;
// 
// 	/** Recalculate sequence length after modifying */
// 	void RecalculateSequenceLength();
// 
// 	/** Clamps the sequence to the specified length
// 	 *
// 	 *  @return		Whether clamping was/is necessary
// 	 */
// 	virtual bool ClampToEndTime(float NewEndTime);
// 
// 	/** Calculates the sequence length of the object
// 	 *
// 	 *  @return		New sequence length
// 	 */
// 	virtual float CalculateSequenceLengthOfEditorObject() const { return GetSequenceLength(); }
// 
// 	/** The slate container that the editor panels are placed in */
// 	TSharedPtr<SVerticalBox>	 EditorPanels;

	/** Slate container used to add controls that are not embedded in a scroll box */
	TSharedPtr<SVerticalBox> NonScrollEditorPanels;
// 
// 	/** The editors Animation Scrub Panel */
// 	TSharedPtr<class SAnimationScrubPanel> AnimScrubPanel;
// 
// 	/** Tracks objects created for the details panel */
// 	FEditorObjectTracker EditorObjectTracker;
// 
// 	/** Delegate called to select objects */
// 	FOnObjectsSelected OnObjectsSelected;
// 
// 	/** The timeline widget */
// 	TSharedPtr<SAnimTimeline> TimelineWidget;
// 
// private:
// 	float GetPercentageInternal() const;
};
