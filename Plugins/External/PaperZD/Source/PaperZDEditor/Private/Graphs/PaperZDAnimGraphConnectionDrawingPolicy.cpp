// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/PaperZDAnimGraphConnectionDrawingPolicy.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

FPaperZDAnimGraphConnectionDrawingPolicy::FPaperZDAnimGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
	: FKismetConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj)
{}

bool FPaperZDAnimGraphConnectionDrawingPolicy::TreatWireAsExecutionPin(UEdGraphPin* InputPin, UEdGraphPin* OutputPin) const
{
	const UPaperZDAnimGraphSchema* Schema = GetDefault<UPaperZDAnimGraphSchema>();
	return (InputPin != nullptr) && (Schema->IsAnimationDataPin(OutputPin->PinType));
}

void FPaperZDAnimGraphConnectionDrawingPolicy::BuildExecutionRoadmap()
{
	//Not yet available
}

void FPaperZDAnimGraphConnectionDrawingPolicy::DetermineStyleOfExecWire(float& Thickness, FLinearColor& WireColor, bool& bDrawBubbles, const FTimePair& Times)
{
	// It's a followed link, make it strong and yellowish but fading over time
	const double BlendWeight = Times.ThisExecTime;

	const float HeavyBlendThickness = AttackWireThickness;
	const float LightBlendThickness = SustainWireThickness;

	Thickness = FMath::Lerp<float>(LightBlendThickness, HeavyBlendThickness, BlendWeight);
	WireColor = WireColor * (BlendWeight * 0.5f + 0.5f);//FMath::Lerp<FLinearColor>(SustainColor, AttackColor, BlendWeight);

	bDrawBubbles = true;
}
