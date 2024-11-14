// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_StateMachine.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "PaperZDAnimGraphNode_StateMachine.generated.h"

class UPaperZDStateMachineGraph;
class SGraphNode;

/**
 * Manages FSM that can drive animations depending of different states and transition rules.
 */
UCLASS()
class UPaperZDAnimGraphNode_StateMachine : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_StateMachine AnimNode;

	/* State machine graph. */
	UPROPERTY()
	TObjectPtr<UPaperZDStateMachineGraph> StateMachineGraph;

	/* Cached name of the state machine. */
	FNodeTextCache CachedFullTitle;

public:
	UPaperZDAnimGraphNode_StateMachine();

	/* Obtains the graph that represents the state machine. */
	FORCEINLINE UPaperZDStateMachineGraph* GetStateMachineGraph() const { return StateMachineGraph; }

	//~ Begin UEdGraphNode Interface.
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual void PostPlacedNewNode() override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	virtual void JumpToDefinition() const override;
	virtual void DestroyNode() override;
	virtual void PostPasteNode() override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;
	virtual void OnRenameNode(const FString& NewName) override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;
	//virtual FString GetDocumentationLink() const override;
	//~ End UEdGraphNode Interface.		

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const;
	virtual void OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData) override;
	//~End UPaperZDAnimGraphNode_Base Interface

	/* Obtains the name of the state machine. */
	FString GetStateMachineName() const;
};
