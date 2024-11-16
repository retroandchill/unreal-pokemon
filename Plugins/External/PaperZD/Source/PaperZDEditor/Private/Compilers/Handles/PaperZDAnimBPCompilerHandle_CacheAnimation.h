// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Compilers/Handles/IPaperZDAnimBPCompilerHandle.h"

class FPaperZDAnimBPCompilerAccess;
class FPaperZDAnimBPGeneratedClassAccess;
class UPaperZDAnimGraphNode_Base;
class UPaperZDAnimGraphNode_CacheAnimation;

/**
 * Handle that pre-processes CacheAnimation nodes and stores them for later linkage with their respective "UseCacheAnimation" nodes.
 */
class FPaperZDAnimBPCompilerHandle_CacheAnimation : public IPaperZDAnimBPCompilerHandle, public TUniqueClassIdentifier<FPaperZDAnimBPCompilerHandle_CacheAnimation>
{	
	/* Map of cache name to encountered "CacheAnimation" nodes. */
	TMap<FString, UPaperZDAnimGraphNode_CacheAnimation*> CacheAnimationNodes;

public:
	//~Begin IPaperZDAnimBPCompilerHandle Interface
	virtual void Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess) override;
	//~End IPaperZDAnimBPCompilerHandle Interface

	/* Called before any animation node has been processed, used to gather the CacheAnimation nodes for their respective "UseCachedAnimation" counterparts. */
	void PreProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilerAccess, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Called after the animation nodes have been processed to do a sanity check and see if there are any circular references between cache nodes.  */
	void PostProcessAnimationNodes(TArrayView<UPaperZDAnimGraphNode_Base*> InAnimNodes, FPaperZDAnimBPCompilerAccess& InCompilerAccess, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);

	/* Checks if no circular references exist on the graph. */
	void CheckForCircularReferences(FPaperZDAnimBPCompilerAccess& InCompilerAccess);

	/* Traverses the all the animation tree connected to the given sink node testing for circular dependencies. */
	void CircularReferenceCheck_TraverseNode(UPaperZDAnimGraphNode_Base* RootNode, FPaperZDAnimBPCompilerAccess& InCompilerAccess, TSet<UPaperZDAnimGraphNode_CacheAnimation*>& VisitedNodes);

	/* Obtains the gathered cache animation nodes obtained in the pre-process stage of the compiler. */
	const TMap<FString, UPaperZDAnimGraphNode_CacheAnimation*>& GetCacheAnimationNodes() const { return CacheAnimationNodes; }
};

//Factory definition
typedef TPaperZDCompilerHandleFactory<FPaperZDAnimBPCompilerHandle_CacheAnimation> FPaperZDCompilerFactory_CacheAnimation;
