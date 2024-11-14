// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "PaperZDAnimGraphSchema.generated.h"

UCLASS(MinimalAPI)
class UPaperZDAnimGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_BODY()

	/* Name of the generated function used to get the exposed pins data. */
	UPROPERTY()
	FName DefaultEvaluationHandlerName;

public:
	UPaperZDAnimGraphSchema(const FObjectInitializer& ObjectInitializer);

	/* Obtain the name of the generated function for the exposed pins. Used in compilation time. */
	FORCEINLINE FName GetDefaultEvaluationHandlerName() const { return DefaultEvaluationHandlerName; }

	//~ Begin UEdGraphSchema Interface.
 	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
 	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;
 	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const override;

	//@NOTE: Needed when the state machine exists and StateGraph nodes become a thing
// 	virtual void HandleGraphBeingDeleted(UEdGraph& GraphBeingRemoved) const override;

	//@NOTE: NOT NEEDED? We don't have conversions between spaces
// 	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const override; 

	//@NOTE: Possibly not used
// 	virtual void DroppedAssetsOnGraph(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
// 	virtual void DroppedAssetsOnNode(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphNode* Node) const override;
// 	virtual void DroppedAssetsOnPin(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphPin* Pin) const override;
// 	virtual void GetAssetsNodeHoverMessage(const TArray<FAssetData>& Assets, const UEdGraphNode* HoverNode, FString& OutTooltipText, bool& OutOkIcon) const override;
// 	virtual void GetAssetsPinHoverMessage(const TArray<FAssetData>& Assets, const UEdGraphPin* HoverPin, FString& OutTooltipText, bool& OutOkIcon) const override;
// 	virtual void GetAssetsGraphHoverMessage(const TArray<FAssetData>& Assets, const UEdGraph* HoverGraph, FString& OutTooltipText, bool& OutOkIcon) const override;
	//@END NOTE

	//@NOTE: No pose watch, so we don't need this
// 	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	//@NOTE: Unless we need to post process the pins, we don't need this
// 	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
 	virtual bool CanDuplicateGraph(UEdGraph* InSourceGraph) const override { return InSourceGraph->GetFName() != UEdGraphSchema_K2::GN_AnimGraph; }
 	virtual bool DoesSupportEventDispatcher() const	override { return false; }
 	virtual bool ShouldAlwaysPurgeOnModification() const override { return true; }
// 	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
// 	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const override;
	//~ End UEdGraphSchema Interface.

	//~ Begin UEdGraphSchema_K2 Interface
 	virtual const FPinConnectionResponse DetermineConnectionResponseOfCompatibleTypedPins(const UEdGraphPin* PinA, const UEdGraphPin* PinB, const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const override;

	//@NOTE: No Local/Component space auto-casting
// 	virtual bool SearchForAutocastFunction(const UEdGraphPin* OutputPin, const UEdGraphPin* InputPin, /*out*/ FName& TargetFunction, /*out*/ UClass*& FunctionOwner) const override;

 	virtual bool ArePinsCompatible(const UEdGraphPin* PinA, const UEdGraphPin* PinB, const UClass* CallingContext = NULL, bool bIgnoreArray = false) const override;
 	virtual bool DoesSupportAnimNotifyActions() const override;

	//@NOTE: No linked interfaces for animation exist here, we don't need to implement
// 	virtual void CreateFunctionGraphTerminators(UEdGraph& Graph, UClass* Class) const override;
	//~ End UEdGraphSchema_K2 Interface

	/* Checks if the given pin is an Animation Data Pin. */
	static bool IsAnimationDataPin(const FEdGraphPinType& PinType);

};
