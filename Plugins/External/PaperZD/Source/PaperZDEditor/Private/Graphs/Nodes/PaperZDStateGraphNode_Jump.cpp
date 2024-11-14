// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode_Jump.h"
#include "Graphs/Nodes/Slate/SPaperZDStateGraphNode_Jump.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Engine/Blueprint.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode_Jump)
#endif

UPaperZDStateGraphNode_Jump::UPaperZDStateGraphNode_Jump(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	Name = TEXT("Jump");
	bCanRenameNode = true;
}

void UPaperZDStateGraphNode_Jump::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();

	//Must make sure the default name isn't used
	TSharedPtr<INameValidatorInterface> Validator = FNameValidatorFactory::MakeValidator(this);
	FString TestName = Name.ToString();
	Validator->FindValidString(TestName);

	//Setup name
	Name = FName(*TestName);
}

void UPaperZDStateGraphNode_Jump::AllocateDefaultPins()
{
	FCreatePinParams PinParams;
	CreatePin(EGPD_Output, TEXT("Transition"), TEXT(""), NULL, TEXT(""), PinParams);
}

TSharedPtr<SGraphNode> UPaperZDStateGraphNode_Jump::CreateVisualWidget()
{ 
	return SNew(SPaperZDStateGraphNode_Jump, this);
}

FText UPaperZDStateGraphNode_Jump::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromName(Name);
}

void UPaperZDStateGraphNode_Jump::OnRenameNode(const FString & NewName)
{
	Name = FName(*NewName);

	//Must mark the blueprint as need-compile, due to our name based indexing
	if (UBlueprint* OwningBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(GetGraph()))
	{		
		FBlueprintEditorUtils::MarkBlueprintAsModified(OwningBlueprint);
	}
}

FString UPaperZDStateGraphNode_Jump::GetNodeName() const
{
	return Name.ToString();
}
