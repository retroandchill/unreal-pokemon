// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"

UEdGraph* FPaperZDAnimBPCompilerAccess::SpawnIntermediateFunctionGraph(const FString& InDesiredFunctionName)
{
	return CompilerContext->SpawnIntermediateFunctionGraph(InDesiredFunctionName);
}

FProperty* FPaperZDAnimBPCompilerAccess::GetAllocatedPropertyFromNode(UPaperZDAnimGraphNode_Base* GraphNode) const
{
	return CompilerContext->AllocatedAnimNodes.FindChecked(GraphNode);
}

FCompilerResultsLog& FPaperZDAnimBPCompilerAccess::GetMessageLog() const
{
	return CompilerContext->MessageLog;
}

void FPaperZDAnimBPCompilerAccess::AddAnimDataLinkRecord(const FPaperZDAnimDataLinkRecord& Record)
{
	CompilerContext->LinkRecords.Add(Record);
}

UEdGraph* FPaperZDAnimBPCompilerAccess::GetConsolidatedEventGraph() const
{
	return CompilerContext->ConsolidatedEventGraph;
}

const UPaperZDAnimBP* FPaperZDAnimBPCompilerAccess::GetAnimBP() const
{
	return CompilerContext->AnimBP;
}

void FPaperZDAnimBPCompilerAccess::ProcessAnimationNodes(TArray<UPaperZDAnimGraphNode_Base*>& AnimNodeList)
{
	CompilerContext->ProcessAnimationNodes(AnimNodeList);
}

void FPaperZDAnimBPCompilerAccess::PruneIsolatedAnimationNodes(const TArray<UPaperZDAnimGraphNode_Base*>& RootSet, TArray<UPaperZDAnimGraphNode_Base*>& GraphNodes)
{
	CompilerContext->PruneIsolatedAnimationNodes(RootSet, GraphNodes);
}

bool FPaperZDAnimBPCompilerAccess::ValidateGraphIsWellFormed(UEdGraph* InGraph) const
{
	return CompilerContext->ValidateGraphIsWellFormed(InGraph);
}

int32 FPaperZDAnimBPCompilerAccess::GetAllocationIndexOfNode(UPaperZDAnimGraphNode_Base* VisualAnimNode) const
{
	int32* pResult = CompilerContext->AllocatedAnimNodeIndices.Find(VisualAnimNode);
	return pResult ? *pResult : INDEX_NONE;
}

void FPaperZDAnimBPCompilerAccess::GetLinkedAnimNodes(UPaperZDAnimGraphNode_Base* InGraphNode, TArray<UPaperZDAnimGraphNode_Base*>& LinkedAnimNodes) const
{
	CompilerContext->GetLinkedAnimNodes(InGraphNode, LinkedAnimNodes);
}

TMap<UPaperZDAnimGraphNode_Base*, UPaperZDAnimGraphNode_Base*>& FPaperZDAnimBPCompilerAccess::GetSourceNodeToProcessedNodeMap() const
{
	return CompilerContext->SourceNodeToProcessedNodeMap;
}

FOnCompileClassSignature& FPaperZDAnimBPCompilerAccess::OnStartCompilingClass()
{
	return CompilerContext->OnStartCompilingClass;
}

FOnCompileClassSignature& FPaperZDAnimBPCompilerAccess::OnFinishCompilingClass()
{
	return CompilerContext->OnFinishCompilingClass;
}

FOnPostExpansionStepSignature& FPaperZDAnimBPCompilerAccess::OnPostExpansionStep()
{
	return CompilerContext->OnPostExpansionStep;
}

FOnCopyTermDefaultsSignature& FPaperZDAnimBPCompilerAccess::OnCopyTermDefaultsToDefaultObject()
{
	return CompilerContext->OnCopyTermDefaultsToDefaultObject;
}

FOnProcessAnimationNodesSignature& FPaperZDAnimBPCompilerAccess::OnPreProcessAnimationNodes()
{
	return CompilerContext->OnPreProcessAnimationNodes;
}

FOnProcessAnimationNodesSignature& FPaperZDAnimBPCompilerAccess::OnPostProcessAnimationNodes()
{
	return CompilerContext->OnPostProcessAnimationNodes;
}
