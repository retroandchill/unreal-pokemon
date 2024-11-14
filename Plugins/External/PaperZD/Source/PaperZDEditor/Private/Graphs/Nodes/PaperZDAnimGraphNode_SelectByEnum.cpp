// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_SelectByEnum.h"
#include "Kismet2/CompilerResultsLog.h"
#include "ToolMenu.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_SelectByEnum)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_SelectByEnum::UPaperZDAnimGraphNode_SelectByEnum()
	: Super()
{
	AnimNode.Animation.AddDefaulted();
}

FText UPaperZDAnimGraphNode_SelectByEnum::GetTooltipText() const
{
	return GetNodeTitle(ENodeTitleType::ListView);
}

FText UPaperZDAnimGraphNode_SelectByEnum::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (BoundEnum == nullptr)
	{
		return LOCTEXT("UPaperZDAnimGraphNode_SelectByEnum_TitleError", "ERROR: Select Animation by (missing enum!!!)");
	}
	// @TODO: don't know enough about this node type to comfortably assert that
	//        the BoundEnum won't change after the node has spawned... until
	//        then, we'll leave this optimization off
	else //if (CachedNodeTitle.IsOutOfDate(this)) 
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("EnumName"), FText::FromString(BoundEnum->GetName()));
		// FText::Format() is slow, so we cache this to save on performance
		CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("UPaperZDAnimGraphNode_SelectByEnum_Title", "Select Animation by ({EnumName})"), Args), this);
	}
	return CachedNodeTitle;
}

FLinearColor UPaperZDAnimGraphNode_SelectByEnum::GetNodeTitleColor() const
{
	return FLinearColor(0.2f, 0.8f, 0.2f);
}

void UPaperZDAnimGraphNode_SelectByEnum::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
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

void UPaperZDAnimGraphNode_SelectByEnum::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading())
	{
		if (BoundEnum != NULL)
		{
			PreloadObject(BoundEnum);
			BoundEnum->ConditionalPostLoad();

			for (auto ExposedIt = VisibleEnumEntries.CreateIterator(); ExposedIt; ++ExposedIt)
			{
				FName& EnumElementName = *ExposedIt;
				const int32 EnumIndex = BoundEnum->GetIndexByName(EnumElementName);

				if (EnumIndex != INDEX_NONE)
				{
					// This handles redirectors, we need to update the VisibleEnumEntries if the name has changed
					FName NewElementName = BoundEnum->GetNameByIndex(EnumIndex);

					if (NewElementName != EnumElementName)
					{
						EnumElementName = NewElementName;
					}
				}
			}
		}
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeEnum(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UEnum> NonConstEnumPtr)
		{
			UPaperZDAnimGraphNode_SelectByEnum* EnumNode = CastChecked<UPaperZDAnimGraphNode_SelectByEnum>(NewNode);
			EnumNode->BoundEnum = NonConstEnumPtr.Get();
		}
	};

	UClass* NodeClass = GetClass();
	// add all blendlist enum entries
	ActionRegistrar.RegisterEnumActions(FBlueprintActionDatabaseRegistrar::FMakeEnumSpawnerDelegate::CreateLambda([NodeClass](const UEnum* Enum)->UBlueprintNodeSpawner*
		{
			UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(NodeClass);
			check(NodeSpawner != nullptr);
			TWeakObjectPtr<UEnum> NonConstEnumPtr = MakeWeakObjectPtr(const_cast<UEnum*>(Enum));
			NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeEnum, NonConstEnumPtr);

			return NodeSpawner;
		}));
}

void UPaperZDAnimGraphNode_SelectByEnum::GetNodeContextMenuActions(UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	if (!Context->bIsDebugging && BoundEnum)
	{
		if (Context->Pin && (Context->Pin->Direction == EGPD_Input))
		{
			int32 RawArrayIndex = 0;
			GetPinInformation(Context->Pin->PinName.ToString(), /*out*/ RawArrayIndex);
			const int32 ExposedEnumIndex = RawArrayIndex - 1;
			if (ExposedEnumIndex != INDEX_NONE)
			{
				// Offer to remove this specific pin
				FUIAction Action = FUIAction(FExecuteAction::CreateUObject(const_cast<UPaperZDAnimGraphNode_SelectByEnum*>(this), &UPaperZDAnimGraphNode_SelectByEnum::RemovePinFromList, const_cast<UEdGraphPin*>(Context->Pin)));
				FToolMenuSection& Section = Menu->AddSection("RemoveAnimation");
				Section.AddMenuEntry("RemoveAnimation", LOCTEXT("RemoveAnimation", "Remove Animation"), FText::GetEmpty(), FSlateIcon(), Action);
			}
		}

		// Offer to add any not-currently-visible pins
		FToolMenuSection* Section = nullptr;
		const int32 MaxIndex = BoundEnum->NumEnums() - 1; // we don't want to show _MAX enum
		for (int32 Index = 0; Index < MaxIndex; ++Index)
		{
			FName ElementName = BoundEnum->GetNameByIndex(Index);
			if (!VisibleEnumEntries.Contains(ElementName))
			{
				FText PrettyElementName = BoundEnum->GetDisplayNameTextByIndex(Index);

				// Offer to add this entry
				if (!Section)
				{
					Section = &Menu->AddSection("AnimGraphNodeAddElementPin", LOCTEXT("ExposeHeader", "Add pin for element"));
				}

				FUIAction Action = FUIAction(FExecuteAction::CreateUObject(const_cast<UPaperZDAnimGraphNode_SelectByEnum*>(this), &UPaperZDAnimGraphNode_SelectByEnum::ExposeEnumElementAsPin, ElementName));
				Section->AddMenuEntry(NAME_None, PrettyElementName, PrettyElementName, FSlateIcon(), Action);
			}
		}
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::PreloadRequiredAssets()
{
	PreloadObject(BoundEnum);

	Super::PreloadRequiredAssets();
}

FString UPaperZDAnimGraphNode_SelectByEnum::GetNodeCategory() const
{
	return TEXT("Select/Blends/Enums");
}

void UPaperZDAnimGraphNode_SelectByEnum::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	Super::ValidateAnimNodeDuringCompilation(ForAnimBP, MessageLog);

	if (BoundEnum == NULL)
	{
		MessageLog.Error(TEXT("@@ references an unknown enum; please delete the node and recreate it"), this);
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::BakeDataDuringCompilation(FCompilerResultsLog& MessageLog)
{
	if (BoundEnum != NULL)
	{
		PreloadObject(BoundEnum);
		BoundEnum->ConditionalPostLoad();

		// Zero the array out so it looks up the default value
		TArray<int32> EnumToAnimationIndex;
		EnumToAnimationIndex.Empty();
		EnumToAnimationIndex.AddZeroed(BoundEnum->NumEnums());;

		// Run thru the enum entries
		int32 PinIndex = 1;
		for (auto ExposedIt = VisibleEnumEntries.CreateConstIterator(); ExposedIt; ++ExposedIt)
		{
			const FName& EnumElementName = *ExposedIt;
			const int32 EnumIndex = BoundEnum->GetIndexByName(EnumElementName);

			if (EnumIndex != INDEX_NONE)
			{
				EnumToAnimationIndex[EnumIndex] = PinIndex;
			}
			else
			{
				MessageLog.Error(*FString::Printf(TEXT("@@ references an unknown enum entry %s"), *EnumElementName.ToString()), this);
			}

			++PinIndex;
		}

		//Pass to the runtime node
		AnimNode.EnumToAnimationIndex = MoveTemp(EnumToAnimationIndex);		
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
	if (ArrayIndex == 0)
	{
		Pin->PinFriendlyName = LOCTEXT("PaperZDSelectByEnum_DefaultNodeName", "Default");
	}
	else if (VisibleEnumEntries.IsValidIndex(ArrayIndex - 1))
	{
		const FName& EnumElementName = VisibleEnumEntries[ArrayIndex - 1];
		const int32 EnumIndex = BoundEnum->GetIndexByName(EnumElementName);
		if (EnumIndex != INDEX_NONE)
		{
			Pin->PinFriendlyName = BoundEnum->GetDisplayNameTextByIndex(EnumIndex);
		}
		else
		{
			Pin->PinFriendlyName = FText::FromName(EnumElementName);
		}
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::RemovePinFromList(UEdGraphPin* Pin)
{
	int32 RawArrayIndex = 0;
	GetPinInformation(Pin->PinName.ToString(), /*out*/ RawArrayIndex);
	const int32 ExposedEnumIndex = RawArrayIndex - 1;
	if (ExposedEnumIndex != INDEX_NONE)
	{
		FScopedTransaction Transaction(LOCTEXT("RemovePin", "RemovePin"));
		Modify();

		// Record it as no longer exposed
		VisibleEnumEntries.RemoveAt(ExposedEnumIndex);

		// Remove the animation from the node
		FProperty* AssociatedProperty;
		int32 ArrayIndex;
		GetPinAssociatedProperty(GetFNodeType(), Pin, /*out*/ AssociatedProperty, /*out*/ ArrayIndex);

		ensure(ArrayIndex == (ExposedEnumIndex + 1));

		// setting up removed pins info 
		RemovedPinIndex = ArrayIndex;
		AnimNode.Animation.RemoveAt(ArrayIndex);
		Pin->SetSavePinIfOrphaned(false);
		ReconstructNode();
		//@TODO: Just want to invalidate the visual representation currently
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::ExposeEnumElementAsPin(FName EnumElementName)
{
	if (!VisibleEnumEntries.Contains(EnumElementName))
	{
		FScopedTransaction Transaction(LOCTEXT("ExposeElement", "ExposeElement"));
		Modify();

		VisibleEnumEntries.Add(EnumElementName);

		AnimNode.Animation.AddDefaulted();

		ReconstructNode();

		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

void UPaperZDAnimGraphNode_SelectByEnum::GetPinInformation(const FString& InPinName, int32& Out_PinIndex)
{
	const int32 UnderscoreIndex = InPinName.Find(TEXT("_"), ESearchCase::CaseSensitive);
	if (UnderscoreIndex != INDEX_NONE)
	{
		const FString ArrayName = InPinName.Left(UnderscoreIndex);
		Out_PinIndex = FCString::Atoi(*(InPinName.Mid(UnderscoreIndex + 1)));
	}
	else
	{
		Out_PinIndex = INDEX_NONE;
	}
}

#undef LOCTEXT_NAMESPACE
