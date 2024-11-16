// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraphSchema_K2.h"
#include "PaperZDAnimTransitionGraphSchema.generated.h"

UCLASS()
class UPaperZDAnimTransitionGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_UCLASS_BODY()
	
	// UEdSchema Interface //
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const override;
	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; }
	virtual bool CanDuplicateGraph(UEdGraph* InSourceGraph) const override { return false; }
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const override;
	
	//The transition graph schema doesn't support local variables and will crash when attempting to right-click a variable inside.
	//The only way to avoid this behavior is for the "DoesSupportLocalVariables" function on the BP editor utils to return false.
	//As usual UE4 has this hard-coded by either checking if the graph is a function or if the graph is an AnimGraph.
	//The only way for us to not crash, is to make sure the graph isn't considered as a function graph, in lieu of not being able to opt-out from
	//having local variables.
	//This is why we return this GraphType
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const { return GT_MAX; }
	// UEdSchema Interface //
};
