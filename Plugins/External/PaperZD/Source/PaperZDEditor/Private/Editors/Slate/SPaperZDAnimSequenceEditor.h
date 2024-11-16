// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Editors/Slate/SPaperZDAnimEditorBase.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "EditorUndoClient.h"

class FPaperZDAnimationSourceEditor;
class UPaperZDAnimSequence;
class FUICommandList;
class FPaperZDAnimModel_AnimSequence;

/**
 * An editor for the animation sequences (track and notifies).
 */
class SPaperZDAnimSequenceEditor : public SPaperZDAnimEditorBase, public FEditorUndoClient
{
	/* Source editor hosting app. */
	TWeakPtr<FPaperZDAnimationSourceEditor> SourceEditorPtr;

	/* Pointer to the animation sequence being edited */
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	/* Pointer to the Anim model we're using right now. */
	TSharedPtr<FPaperZDAnimModel_AnimSequence> AnimModel;

public:
	SLATE_BEGIN_ARGS(SPaperZDAnimSequenceEditor)
		: _AnimSequence(nullptr)
	{}
	SLATE_ARGUMENT(UPaperZDAnimSequence*, AnimSequence)
	SLATE_EVENT(FPaperZDEditorDelegates::OnObjectsSelectedSignature, OnObjectsSelected)
	SLATE_EVENT(FPaperZDEditorDelegates::OnInvokeTabSignature, OnInvokeTab)
	SLATE_END_ARGS()

	/* Constructs the widget. */
	void Construct(const FArguments& InArgs, TSharedPtr<FPaperZDAnimationSourceEditor> InSourceEditor);
	~SPaperZDAnimSequenceEditor();

	//~Begin SPaperZDAnimEditorBase Interface
	virtual UPaperZDAnimSequence* GetEditorObject() const override { return AnimSequence; }
	//~End SPaperZDAnimEditorBase Interface

private:
	//~Begin FEditorUndoClient interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	//~End FEditorUndoClient interface

	/** Post undo **/
	void PostUndoRedo();
};
