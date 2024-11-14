// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintConnectionDrawingPolicy.h"

class FSlateWindowElementList;
class UEdGraph;

/**
 * Draws the connection for the animation data link.
 */
class FPaperZDAnimGraphConnectionDrawingPolicy : public FKismetConnectionDrawingPolicy
{
public:
	// Constructor
	FPaperZDAnimGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);

	// FKismetConnectionDrawingPolicy interface
	virtual bool TreatWireAsExecutionPin(UEdGraphPin* InputPin, UEdGraphPin* OutputPin) const override;
	virtual void BuildExecutionRoadmap() override;
	virtual void DetermineStyleOfExecWire(float& Thickness, FLinearColor& WireColor, bool& bDrawBubbles, const FTimePair& Times) override;
	// End of FKismetConnectionDrawingPolicy interface
};
