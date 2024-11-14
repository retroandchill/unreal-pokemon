// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDAnimGraphNode_OverrideSlot.h"
#include "Compilers/Access/PaperZDAnimBPCompilerAccess.h"
 #include "Compilers/Access/PaperZDAnimBPGeneratedClassAccess.h"
 #include "Kismet2/CompilerResultsLog.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimGraphNode_OverrideSlot)
#endif

#define LOCTEXT_NAMESPACE "ZDNodes"

UPaperZDAnimGraphNode_OverrideSlot::UPaperZDAnimGraphNode_OverrideSlot()
	: Super()
{}

FString UPaperZDAnimGraphNode_OverrideSlot::GetNodeCategory() const
{
	return TEXT("Slots");
}

void UPaperZDAnimGraphNode_OverrideSlot::ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog)
{
	//Make sure we're not sending empty names
	if (AnimNode.SlotName.IsNone() || AnimNode.GroupName.IsNone())
	{
		MessageLog.Error(*LOCTEXT("NoSlotOrGroupName", "Slot name or group cannot be empty").ToString());
		return;
	}
}

void UPaperZDAnimGraphNode_OverrideSlot::OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData)
{
	TMap<FName, FPaperZDOverrideSlotDescriptor>& SlotDescriptors = OutCompiledData.GetRegisteredOverrideSlots();
	FPaperZDOverrideSlotDescriptor* pDescriptor = SlotDescriptors.Find(AnimNode.SlotName);

	//Register only if the slot hasn't been registered by another node
	if (!pDescriptor)
	{
		FPaperZDOverrideSlotDescriptor NewDescriptor;
		NewDescriptor.SlotName = AnimNode.SlotName;
		NewDescriptor.GroupName = AnimNode.GroupName;
		
		//Key by the slot name for easier runtime searches
		SlotDescriptors.Add(NewDescriptor.SlotName, NewDescriptor);
	}
	else if ((*pDescriptor).GroupName != (AnimNode.GroupName))
	{
		InCompilationContext.GetMessageLog().Error(*FText::Format(LOCTEXT("InvalidSlotGroupName", "Found clashing group names '{0}' and '{1}'. Node '@@'"), FText::FromName((*pDescriptor).GroupName), FText::FromName(AnimNode.GroupName)).ToString(), this);
	}
}

FText UPaperZDAnimGraphNode_OverrideSlot::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::EditableTitle)
	{
		return FText::FromName(AnimNode.SlotName);
	}
	else if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("NewOverrideSlot", "New Override Slot...");
	}
	else if (CachedNodeTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("SlotName"), FText::FromName(AnimNode.SlotName));
		Args.Add(TEXT("GroupName"), FText::FromName(AnimNode.GroupName));
		return FText::Format(LOCTEXT("ZDAnimGraphNode_OverrideSlot_Title", "Override Slot '{SlotName}' \nGroup '{GroupName}'"), Args);
	}

	return CachedNodeTitle;
}

FLinearColor UPaperZDAnimGraphNode_OverrideSlot::GetNodeTitleColor() const
{
	return FLinearColor(0.7f, 0.7f, 0.7f);
}

#undef LOCTEXT_NAMESPACE
