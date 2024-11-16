// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphSchema.h"
#include "PaperZDStateMachineSchema.generated.h"

/** Action to add a node to the graph */
USTRUCT()
struct FPaperZDStateMachineSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	/** Class of node we want to create */
	UPROPERTY()
	UClass* NodeClass;

	FPaperZDStateMachineSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, NodeClass(nullptr)
	{}

	FPaperZDStateMachineSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
		, NodeClass(nullptr)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
	
	template<typename T>
	static T* SpawnGraphNode(class UEdGraph* ParentGraph, const FVector2D Location = FVector2D(0.0f, 0.0f), bool bSelectNewNode = true)
	{
		FPaperZDStateMachineSchemaAction_NewNode Action;
		Action.NodeClass = T::StaticClass();
		return Cast<T>(Action.PerformAction(ParentGraph, nullptr, Location, bSelectNewNode));
	}
};

USTRUCT()
struct FPaperZDStateMachineSchemaAction_NewComment : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FPaperZDStateMachineSchemaAction_NewComment()
		: FEdGraphSchemaAction()
	{}

	FPaperZDStateMachineSchemaAction_NewComment(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface
};


UCLASS(MinimalAPI)
class UPaperZDStateMachineSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	UPaperZDStateMachineSchema(const FObjectInitializer& ObjectInitializer);

	// UEdSchema Interface //
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override; 
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const override;
	virtual void GetGraphDisplayInformation(const UEdGraph & Graph, FGraphDisplayInfo & DisplayInfo) const override;
	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const { return GT_StateMachine; }
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	// UEdSchema Interface //
};
