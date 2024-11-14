// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraphSchema_K2.h"
#include "PaperZDNotifyGraphSchema.generated.h"

/**
 * Schema for the AnimNotify graph
 */
UCLASS()
class UPaperZDNotifyGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_UCLASS_BODY()

	// UEdSchema Interface //
	//virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; }
	virtual bool CanDuplicateGraph(UEdGraph* InSourceGraph) const override { return false; }
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const override;
	// UEdSchema Interface //
};
