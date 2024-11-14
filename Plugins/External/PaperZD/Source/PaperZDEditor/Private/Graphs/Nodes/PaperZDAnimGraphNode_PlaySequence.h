// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_PlaySequence.h"
#include "PaperZDAnimGraphNode_PlaySequence.generated.h"

class UPaperZDAnimSequence;

/**
 * Plays an Animation Sequence.
 */
UCLASS()
class UPaperZDAnimGraphNode_PlaySequence : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Friendship for version support. */
	friend class FPaperZDRuntimeEditorProxy;

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_PlaySequence AnimNode;

	/* The path to the animation source that owns the sequence, used on PROTO_NODEs which can point to unloaded assets. */
	FString PROTO_AnimSourceTag;

public:
	UPaperZDAnimGraphNode_PlaySequence();

	/* Obtain the animation asset bound to this node. */
	UPaperZDAnimSequence* GetAnimSequence() const { return AnimNode.AnimSequence; }

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog);
	//~End UPaperZDAnimGraphNode_Base Interface

	//~Begin UK2Node Interface
	virtual void PreloadRequiredAssets() override;
	//~End UK2Node Interface

	//~ Begin UEdGraphNode Interface.
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
	virtual void PostPlacedNewNode() override;
	virtual void PostPasteNode() override;
	//~ End UEdGraphNode Interface.

private:
	/* Obtains the title of the node, by the given sequence. Can be null. */
	FText GetNodeTitleBySequenceName(FName SequenceName) const;

	/* Obtains the tooltip of the node, by the given sequence. Can be null. */
	FText GetNodeTooltipBySequencePath(FString SequencePath) const;
};
