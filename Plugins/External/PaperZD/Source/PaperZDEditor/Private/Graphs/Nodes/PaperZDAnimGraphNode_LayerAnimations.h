// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_LayerAnimations.h"
#include "PaperZDAnimGraphNode_LayerAnimations.generated.h"

/**
 * Sets the animations in different weighted layers, running parallel and independent to each other.
 */
UCLASS()
class UPaperZDAnimGraphNode_LayerAnimations : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_LayerAnimations AnimNode;

	/* When a pin has been removed, it holds the index that points to it, so that node reconstruction can correctly re-bind the links. */
	int32 RemovedPinIndex;

public:
	UPaperZDAnimGraphNode_LayerAnimations();

	//~ Begin UEdGraphNode Interface.
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	//~ End UEdGraphNode Interface.		

	// UK2Node interface
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	// End of UK2Node interface

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog) override;
	virtual bool IsCompatibleWithAnimationSource(UPaperZDAnimationSource* InAnimSource) const override;
	//~End UPaperZDAnimGraphNode_Base Interface

	/* Adds an animation pin to the entry list. */
	void AddLayerPin();

	/* Removes the given animation pin. */
	void RemoveLayerPin(UEdGraphPin* Pin);
};
