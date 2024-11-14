// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/Slate/SPaperZDGraphPin_AnimDataLink.h"
#include "Editors/Util/PaperZDEditorStyle.h"

/////////////////////////////////////////////////////
// SPaperZDGraphPin_AnimDataLink

void SPaperZDGraphPin_AnimDataLink::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);

	CachedImg_Pin_ConnectedHovered = FPaperZDEditorStyle::GetBrush(TEXT("PaperZDEditor.Graph.AnimationPin.ConnectedHovered"));
	CachedImg_Pin_Connected = FPaperZDEditorStyle::GetBrush(TEXT("PaperZDEditor.Graph.AnimationPin.Connected"));
	CachedImg_Pin_DisconnectedHovered = FPaperZDEditorStyle::GetBrush(TEXT("PaperZDEditor.Graph.AnimationPin.DisconnectedHovered"));
	CachedImg_Pin_Disconnected = FPaperZDEditorStyle::GetBrush(TEXT("PaperZDEditor.Graph.AnimationPin.Disconnected"));
}

const FSlateBrush* SPaperZDGraphPin_AnimDataLink::GetPinIcon() const
{
	const FSlateBrush* Brush = NULL;

	if (IsConnected())
	{
		Brush = IsHovered() ? CachedImg_Pin_ConnectedHovered : CachedImg_Pin_Connected;
	}
	else
	{
		Brush = IsHovered() ? CachedImg_Pin_DisconnectedHovered : CachedImg_Pin_Disconnected;
	}

	return Brush;
}
