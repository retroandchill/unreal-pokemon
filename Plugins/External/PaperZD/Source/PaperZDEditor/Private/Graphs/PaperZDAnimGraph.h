// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "PaperZDAnimGraph.generated.h"

/**
 * Base class for the Animation Graph for PaperZD
 */
UCLASS(MinimalAPI)
class UPaperZDAnimGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	/** Returns the AnimBP that contains this graph */
	class UPaperZDAnimBP* GetAnimBP() const;

	/**
	 * If true, this graph is considered a "transitional" graph which only plays for a single animation loop.
	 * Transitional graphs should have no state machines nor looping PlaySequence nodes.
	 */
	 bool IsTransitionalGraph() const;
};
