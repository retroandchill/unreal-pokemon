// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_SelectByEnum.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "PaperZDAnimGraphNode_SelectByEnum.generated.h"

/**
 * Selects between two animations depending on a given Enum value.
 */
UCLASS()
class UPaperZDAnimGraphNode_SelectByEnum : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_SelectByEnum AnimNode;

	/* Enum being switched on */
	UPROPERTY()
	TObjectPtr<UEnum> BoundEnum;

	/* List of enum entries this graph is exposing. */
	UPROPERTY()
	TArray<FName> VisibleEnumEntries;

	/* Constructing FText strings can be costly, so we cache the node's title. (Currently disabled) */
	FNodeTextCache CachedNodeTitle;

	/* When a pin has been removed, it holds the index that points to it, so that node reconstruction can correctly re-bind the links. */
	int32 RemovedPinIndex;

public:
	UPaperZDAnimGraphNode_SelectByEnum();

	//~ Begin UEdGraphNode Interface.
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End UEdGraphNode Interface.

	// UK2Node interface
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual void PreloadRequiredAssets() override;
	// End of UK2Node interface

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog);
	virtual void BakeDataDuringCompilation(FCompilerResultsLog& MessageLog) override;
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const override;
	//~End UPaperZDAnimGraphNode_Base Interface

private:
	void RemovePinFromList(UEdGraphPin* Pin);

	// Exposes a pin corresponding to the specified element name
	void ExposeEnumElementAsPin(FName EnumElementName);

	// Gets information about the specified pin.
	static void GetPinInformation(const FString& InPinName, int32& Out_PinIndex);
};
