// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimSequenceEditor.h"
#include "Editors/AnimTimeline/PaperZDAnimModel_AnimSequence.h"
#include "Editors/PaperZDAnimationSourceEditor.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "PaperZDEditor_AnimSequenceEditor"

void SPaperZDAnimSequenceEditor::Construct(const FArguments& InArgs, TSharedPtr<FPaperZDAnimationSourceEditor> InSourceEditor)
{
	SourceEditorPtr = InSourceEditor;
	AnimSequence = InArgs._AnimSequence;
	check(AnimSequence);

	AnimModel = MakeShared<FPaperZDAnimModel_AnimSequence>(InSourceEditor->GetToolkitCommands(), AnimSequence, InSourceEditor->GetAnimationSourceBeingEdited(), InSourceEditor->GetViewportClient().Pin());	
	AnimModel->OnSelectObjects = FPaperZDEditorDelegates::OnObjectsSelectedSignature::CreateSP(this, &SPaperZDAnimEditorBase::OnSelectionChanged);
	AnimModel->OnInvokeTab = InArgs._OnInvokeTab;
	AnimModel->Initialize();

	SPaperZDAnimEditorBase::Construct(SPaperZDAnimEditorBase::FArguments()
		.OnObjectsSelected(InArgs._OnObjectsSelected)
		.AnimModel(AnimModel), 
		InSourceEditor);

	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}
}

SPaperZDAnimSequenceEditor::~SPaperZDAnimSequenceEditor()
{
	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}
}

void SPaperZDAnimSequenceEditor::PostUndo(bool bSuccess)
{
	PostUndoRedo();
}

void SPaperZDAnimSequenceEditor::PostRedo(bool bSuccess)
{
	PostUndoRedo();
}

void SPaperZDAnimSequenceEditor::PostUndoRedo()
{
	AnimModel->RefreshTracks();
}

#undef LOCTEXT_NAMESPACE
