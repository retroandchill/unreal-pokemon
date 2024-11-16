// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_UseCachedAnimation.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_CacheAnimation.h"
#include "Compilers/Handles/PaperZDAnimBPCompilerHandle_CacheAnimation.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/CompilerResultsLog.h"
#include "BlueprintActionFilter.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_UseCachedAnimation)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_UseCachedAnimation::UPaperZDAnimGraphNode_UseCachedAnimation()
	: Super()
{}

FString UPaperZDAnimGraphNode_UseCachedAnimation::GetNodeCategory() const
{
	return TEXT("Cached Animations");
}

void UPaperZDAnimGraphNode_UseCachedAnimation::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	bool bRefreshCacheNode = true;

	//Check to see if the current cached node is still valid (and not deleted, by checking pin connections)
	if (LinkedCacheNode.IsValid())
	{
		// The node has a single pin, make sure it's there
 		check(LinkedCacheNode->Pins.Num());

		// Deleted nodes have no links, otherwise we will be doing some wasted work on unlinked nodes
		if (LinkedCacheNode->Pins[0]->LinkedTo.Num())
		{
			// The node has links, it's valid, continue to use it
			bRefreshCacheNode = false;
		}
	}

	// We need to refresh the cached pose node this node is linked to
	if (bRefreshCacheNode && !NameOfCache.IsEmpty())
	{
		UBlueprint* GraphBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph());
		check(GraphBlueprint);

		TArray<UEdGraph*> AllGraphs;
		GraphBlueprint->GetAllGraphs(AllGraphs);

		for (UEdGraph* Graph : AllGraphs)
		{
			// Get a list of all save cached pose nodes
			TArray<UPaperZDAnimGraphNode_CacheAnimation*> CacheAnimationNodes;
			Graph->GetNodesOfClass(CacheAnimationNodes);

			// Go through all the nodes and find one with a title that matches ours
			for (auto NodeIt = CacheAnimationNodes.CreateIterator(); NodeIt; ++NodeIt)
			{
				if ((*NodeIt)->GetCacheName() == NameOfCache)
				{
					// Fix the original Blueprint node as well as the compiled version
					MessageLog.FindSourceObjectTypeChecked<UPaperZDAnimGraphNode_UseCachedAnimation>(this)->LinkedCacheNode = *NodeIt;
					LinkedCacheNode = *NodeIt;
					break;
				}
			}
		}
	}
}

void UPaperZDAnimGraphNode_UseCachedAnimation::OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	FPaperZDAnimBPCompilerHandle_CacheAnimation* Handle = InCompilationContext.GetHandle<FPaperZDAnimBPCompilerHandle_CacheAnimation>();
	check(Handle);

	bool bSuccess = false;
	if (LinkedCacheNode.IsValid())
	{
		//Need to create a link between this node and the compiler's "true node" CacheAnimation node
		UPaperZDAnimGraphNode_CacheAnimation* CacheAnimationNode = Handle->GetCacheAnimationNodes().FindRef(LinkedCacheNode->GetCacheName());
		if (CacheAnimationNode)
		{
			FStructProperty* LinkProperty = FindFProperty<FStructProperty>(FPaperZDAnimNode_UseCachedAnimation::StaticStruct(), TEXT("LinkedCacheNode"));
			check(LinkProperty);

			FPaperZDAnimDataLinkRecord Record = FPaperZDAnimDataLinkRecord::MakeFromMember(this, CacheAnimationNode, LinkProperty);
			if (Record.IsValid())
			{
				InCompilationContext.AddAnimDataLinkRecord(Record);
				bSuccess = true;
			}
		}			
	}

	if(!bSuccess)
	{
		InCompilationContext.GetMessageLog().Error(*LOCTEXT("NoAssociatedCacheNode", "@@ does not have an associated Cache Animation node").ToString(), this);
	}
}

FText UPaperZDAnimGraphNode_UseCachedAnimation::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FFormatNamedArguments Args;
	if (LinkedCacheNode.IsValid())
	{
		NameOfCache = LinkedCacheNode->GetCacheName();
	}
	Args.Add(TEXT("CachedAnimationName"), FText::FromString(NameOfCache));
	return FText::Format(LOCTEXT("ZDAnimGraphNode_UseCachedAnimation_Title", "Use cached animation '{CachedAnimationName}'"), Args);
}

void UPaperZDAnimGraphNode_UseCachedAnimation::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	auto PostSpawnSetupLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode, FString CacheNodeName, UPaperZDAnimGraphNode_CacheAnimation* CacheAnimationNode)
	{
		UPaperZDAnimGraphNode_UseCachedAnimation* UseCachedAnimation = CastChecked<UPaperZDAnimGraphNode_UseCachedAnimation>(NewNode);
		// we use an empty CacheName in GetNodeTitle() to relay the proper menu title
		UseCachedAnimation->LinkedCacheNode = CacheAnimationNode;
	};


	UObject const* ActionKey = ActionRegistrar.GetActionKeyFilter();

	if (UBlueprint const* Blueprint = Cast<UBlueprint>(ActionKey))
	{
		// Get a list of all save cached pose nodes
		TArray<UPaperZDAnimGraphNode_CacheAnimation*> CachedAnimationNodes;
		FBlueprintEditorUtils::GetAllNodesOfClass<UPaperZDAnimGraphNode_CacheAnimation>(Blueprint, /*out*/ CachedAnimationNodes);

		// Offer a use node for each of them with our layer in its outer chain
		for (UPaperZDAnimGraphNode_CacheAnimation* CacheNode : CachedAnimationNodes)
		{
			UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
			NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(PostSpawnSetupLambda, CacheNode->GetCacheName(), CacheNode);

			ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
		}
	}
}

bool UPaperZDAnimGraphNode_UseCachedAnimation::IsActionFilteredOut(class FBlueprintActionFilter const& Filter)
{
	bool bIsFilteredOut = false;
	if (LinkedCacheNode.IsValid())
	{
		FBlueprintActionContext const& FilterContext = Filter.Context;
		for (UBlueprint* Blueprint : FilterContext.Blueprints)
		{
			if (LinkedCacheNode->GetBlueprint() != Blueprint)
			{
				bIsFilteredOut = true;
				break;
			}
		}
	}
	return bIsFilteredOut;
}

#undef LOCTEXT_NAMESPACE
