// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/Slate/SPaperZDAnimGraphNode_Sink.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "Editors/Util/PaperZDEditorStyle.h"

/////////////////////////////////////////////////////
// SGraphNodeAnimationResult

void SPaperZDAnimGraphNode_Sink::Construct(const FArguments& InArgs, UPaperZDAnimGraphNode_Base* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

TSharedRef<SWidget> SPaperZDAnimGraphNode_Sink::CreateNodeContentArea()
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(0, 3))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.FillWidth(1.0f)
		[
			// LEFT
			SAssignNew(LeftNodeBox, SVerticalBox)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(FPaperZDEditorStyle::GetBrush("PaperZDEditor.Graph.SinkNode.ResultImage"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		[
			// RIGHT
			SAssignNew(RightNodeBox, SVerticalBox)
		]
		];
}

