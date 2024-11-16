// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_RandomPlayer.h"
#include "AnimSequences/PaperZDAnimSequence.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_RandomPlayer)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_RandomPlayer::UPaperZDAnimGraphNode_RandomPlayer()
	: Super()
{}

FString UPaperZDAnimGraphNode_RandomPlayer::GetNodeCategory() const
{
	return TEXT("Animation");
}

FLinearColor UPaperZDAnimGraphNode_RandomPlayer::GetNodeTitleColor() const
{
	return FLinearColor(0.10f, 0.60f, 0.12f);
}

FText UPaperZDAnimGraphNode_RandomPlayer::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("PlayRandomSequence_Title", "Play Random Sequence");
}

#undef LOCTEXT_NAMESPACE