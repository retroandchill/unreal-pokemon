// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimationSourceViewport.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include  "Widgets/SNullWidget.h"

void SPaperZDAnimationSourceViewport::Construct(const FArguments& InArgs, const UPaperZDAnimationSource& InAnimationSource)
{
	AnimationSourcePtr = &InAnimationSource;
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SPaperZDAnimationSourceViewport::BindCommands()
{
	SEditorViewport::BindCommands();

	//@TODO: possible commands as: Show Grid, Show collisions, etc
}

TSharedRef<FEditorViewportClient> SPaperZDAnimationSourceViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FPaperZDAnimationSourceViewportClient(AnimationSourcePtr));
	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SPaperZDAnimationSourceViewport::MakeViewportToolbar()
{
	return SNew(SVerticalBox);
}

EVisibility SPaperZDAnimationSourceViewport::GetTransformToolbarVisibility() const
{
	return EVisibility::Visible;
}

void SPaperZDAnimationSourceViewport::OnFocusViewportToSelection()
{
	//@TODO: add better focus on selecting
	//EditorViewportClient->RequestFocusOnSelection(/*bInstant=*/ false);
}

TSharedRef<class SEditorViewport> SPaperZDAnimationSourceViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SPaperZDAnimationSourceViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SPaperZDAnimationSourceViewport::OnFloatingButtonClicked()
{
}

TSharedRef<FPaperZDAnimationSourceViewportClient> SPaperZDAnimationSourceViewport::GetEditorViewportClient()
{
	check(EditorViewportClient.IsValid());
	return EditorViewportClient.ToSharedRef();
}
