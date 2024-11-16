// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_SelectByInt.h"
#include "Kismet2/CompilerResultsLog.h"
#include "ToolMenu.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_SelectByInt)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_SelectByInt::UPaperZDAnimGraphNode_SelectByInt()
	: Super()
	, RemovedPinIndex(INDEX_NONE)
{
	//Make sure we have enough pins
	AnimNode.Animation.AddDefaulted(2);
}

FText UPaperZDAnimGraphNode_SelectByInt::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Select Animation by Int"));
}

FLinearColor UPaperZDAnimGraphNode_SelectByInt::GetNodeTitleColor() const
{
	return FLinearColor(0.2f, 0.8f, 0.2f);
}

void UPaperZDAnimGraphNode_SelectByInt::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	//If the reconstruction is due to a pending pin deletion, then we must reorder the connection list to patch it up
	if (RemovedPinIndex != INDEX_NONE)
	{
		TArray<FString> RemovedPropertyNames;
		TArray<FName> NewPinNames;

		// Store new pin names to compare with old pin names
		for (int32 NewPinIndx = 0; NewPinIndx < Pins.Num(); NewPinIndx++)
		{
			NewPinNames.Add(Pins[NewPinIndx]->PinName);
		}

		// don't know which pins are removed yet so find removed pins comparing NewPins and OldPins
		for (int32 OldPinIdx = 0; OldPinIdx < OldPins.Num(); OldPinIdx++)
		{
			const FName OldPinName = OldPins[OldPinIdx]->PinName;
			if (!NewPinNames.Contains(OldPinName))
			{
				const FString OldPinNameStr = OldPinName.ToString();
				const int32 UnderscoreIndex = OldPinNameStr.Find(TEXT("_"), ESearchCase::CaseSensitive);
				if (UnderscoreIndex != INDEX_NONE)
				{
					FString PropertyName = OldPinNameStr.Left(UnderscoreIndex);
					RemovedPropertyNames.Add(MoveTemp(PropertyName));
				}
			}
		}

		for (int32 PinIdx = 0; PinIdx < OldPins.Num(); PinIdx++)
		{
			// Separate the pin name into property name and index
			const FString OldPinNameStr = OldPins[PinIdx]->PinName.ToString();
			const int32 UnderscoreIndex = OldPinNameStr.Find(TEXT("_"), ESearchCase::CaseSensitive);

			if (UnderscoreIndex != INDEX_NONE)
			{
				const FString PropertyName = OldPinNameStr.Left(UnderscoreIndex);

				if (RemovedPropertyNames.Contains(PropertyName))
				{
					const int32 ArrayIndex = FCString::Atoi(*(OldPinNameStr.Mid(UnderscoreIndex + 1)));

					// if array index is matched, removes pins 
					// and if array index is greater than removed index, decrease index
					if (ArrayIndex == RemovedPinIndex)
					{
						OldPins[PinIdx]->MarkAsGarbage();
						OldPins.RemoveAt(PinIdx);
						--PinIdx;
					}
					else if (ArrayIndex > RemovedPinIndex)
					{
						OldPins[PinIdx]->PinName = *FString::Printf(TEXT("%s_%d"), *PropertyName, ArrayIndex - 1);
					}
				}
			}
		}

		//Pin removal complete
		RemovedPinIndex = INDEX_NONE;
	}
}

void UPaperZDAnimGraphNode_SelectByInt::GetNodeContextMenuActions(UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	if (!Context->bIsDebugging)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("PaperZDSelectByInt", NSLOCTEXT("ZDNodes", "SelectListHeader", "PaperZD"));
			if (Context->Pin != NULL)
			{
				if (Context->Pin->Direction == EGPD_Input)
				{
					Section.AddMenuEntry(
						"RemoveAnimationListPin",
						LOCTEXT("RemoveAnimationListPin_Title", "Remove animation pin."),
						LOCTEXT("RemoveAnimationListPin_Tooltip", "Removes the given pin from the animation entry list."),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateUObject(const_cast<UPaperZDAnimGraphNode_SelectByInt*>(this), &UPaperZDAnimGraphNode_SelectByInt::RemoveAnimationPin, const_cast<UEdGraphPin*>(Context->Pin)))
					);
				}
			}
			else
			{
				Section.AddMenuEntry(
					"AddAnimationListPin",
					LOCTEXT("AddAnimationListPin_Title", "Add animation pin."),
					LOCTEXT("AddAnimationListPin_Tooltip", "Add a new animation entry to the list."),
					FSlateIcon(),
					FUIAction(FExecuteAction::CreateUObject(const_cast<UPaperZDAnimGraphNode_SelectByInt*>(this), &UPaperZDAnimGraphNode_SelectByInt::AddAnimationPin))
				);
			}
		}
	}
}

FString UPaperZDAnimGraphNode_SelectByInt::GetNodeCategory() const
{
	return TEXT("Select/Blends");
}

void UPaperZDAnimGraphNode_SelectByInt::AddAnimationPin()
{
	FScopedTransaction Transaction(NSLOCTEXT("ZDNodes", "AddAnimationListPin", "Add Animation Pin"));
	Modify();

	AnimNode.Animation.AddDefaulted();
	ReconstructNode();

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UPaperZDAnimGraphNode_SelectByInt::RemoveAnimationPin(UEdGraphPin* Pin)
{
	FScopedTransaction Transaction(NSLOCTEXT("ZDNodes", "RemoveAnimationListPin", "Remove Animation Pin"));
	Modify();

	FProperty* AssociatedProperty;
	int32 ArrayIndex;
	GetPinAssociatedProperty(GetFNodeType(), Pin, /*out*/ AssociatedProperty, /*out*/ ArrayIndex);

	if (ArrayIndex != INDEX_NONE)
	{
		RemovedPinIndex = ArrayIndex;
		AnimNode.Animation.RemoveAt(ArrayIndex);
		ReconstructNode();
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

#undef LOCTEXT_NAMESPACE
